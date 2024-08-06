#include "../../include/renderer/pass/ImmediateQuadRenderPass.hpp"

#include "mono/util/IndexOf.hpp"

namespace mono::renderer
{

ImmediateQuadRenderPass::ImmediateQuadRenderPass(
    std::shared_ptr<mono::gl::RenderTarget> render_target,
    mono::gl::ShaderProgram& shader)
    : m_renderTarget(std::move(render_target))
    , m_shader(shader)
    , m_quadVao(std::make_shared<gl::VertexArray>())
{
    this->prepareQuadVao();
    this->prepareQuadSsbo();
}

void ImmediateQuadRenderPass::clear()
{
    m_quads.clear();
}

std::shared_ptr<mono::gl::VertexArray> ImmediateQuadRenderPass::getVao()
{
    return m_quadVao;
}

std::shared_ptr<mono::gl::ShaderProgram> ImmediateQuadRenderPass::getShader()
{
    return std::shared_ptr<gl::ShaderProgram>{&m_shader};
}

std::shared_ptr<mono::gl::RenderTarget> ImmediateQuadRenderPass::getRenderTarget()
{
    return m_renderTarget;
}

void ImmediateQuadRenderPass::submitDraws(const glm::mat4& projection, const glm::mat4& view)
{
    if(not m_quads.empty())
    {
        m_quadSsbo->setData(m_quads);

        // prepare uniform data
        constexpr std::array<std::int32_t, 32> samplers{0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
                                                        11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
                                                        22, 23, 24, 25, 26, 27, 28, 29, 30, 31};
        std::array<std::uint32_t, 32> frame_counts{};
        std::array<std::uint32_t, 32> frame_row_lengths{};
        std::array<std::uint32_t, 32> frame_current_indices{};

        for(std::size_t slot = 0; slot < m_textures.size(); slot++)
        {
            // BUG: CAN GO OUT OF BOUND IF MORE THAN 32 TEXTURES!
            const auto& texture = m_textures[slot];
            const auto& id = texture->getID();
            glBindTextureUnit(slot, id);  // slot = unit

            const auto& tex_data = texture->getTextureData();
            frame_counts.at(slot) = tex_data.numberOfSubs;
            frame_row_lengths.at(slot) = tex_data.numberOfSubsInOneRow;
            frame_current_indices.at(slot) = tex_data.currentSub;
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        m_quadSsbo->bind(0);

        m_shader.use();

        m_shader.uploadUniform("uProjection", projection, 0);
        m_shader.uploadUniform("uView", view, 1);

        m_shader.uploadUniform("uTextures", samplers, 2);
        m_shader.uploadUniform("uFrameCount", frame_counts, 34);
        m_shader.uploadUniform("uFrameRowLength", frame_row_lengths, 66);
        m_shader.uploadUniform("uFrameCurrentIndex", frame_current_indices, 98);

        m_quadVao->bind();
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr, m_quads.size());
        m_quadVao->unbind();
        m_quadSsbo->unbind();

        glDisable(GL_BLEND);

        for(std::size_t slot = 0; slot < m_textures.size(); slot++)
        {
            glBindTextureUnit(slot, 0);
        }

        this->clear();
    }
}

void ImmediateQuadRenderPass::drawQuad(
    const glm::vec2& position,
    const glm::vec2& size,
    float rotation,
    std::shared_ptr<Texture> texture,
    const glm::vec4& color)
{
    std::uint32_t color_uint = 0u;
    color_uint |= static_cast<std::uint32_t>(color.r * 255) << 24;
    color_uint |= static_cast<std::uint32_t>(color.g * 255) << 16;
    color_uint |= static_cast<std::uint32_t>(color.b * 255) << 8;
    color_uint |= static_cast<std::uint32_t>(color.a * 255);

    const auto texture_id = texture->getID();
    const std::size_t texture_slot = [this, &texture, texture_id]() -> std::size_t {
        const auto slot = util::indexOf(m_textures, [texture_id](const auto& texture) {
            return texture->getID() == texture_id;
        });
        if(not slot)
        {
            m_textures.push_back(std::move(texture));
            return m_textures.size() - 1;
        }
        return slot.value();
    }();

    const gl::QuadInstanceData quad_instance_data{
        color_uint,
        position,
        size,
        gl::detail::RtiPacked{static_cast<glm::uint32>(rotation), texture_slot}
    };

    m_quads.push_back(quad_instance_data);
}

void ImmediateQuadRenderPass::drawQuad(
    const glm::vec2& position,
    const glm::vec2& size,
    float rotation,
    const glm::vec4& color)
{
    static std::shared_ptr<Texture> white_texture = std::make_shared<Texture>(
        std::array<std::byte, 4>{std::byte{0xff}, std::byte{0xff}, std::byte{0xff}, std::byte{0xff}}
            .data(),
        1,
        1);
    return this->drawQuad(position, size, rotation, white_texture, color);
}

void ImmediateQuadRenderPass::prepareQuadVao()
{
    auto quad_constant_vbo = gl::VertexBuffer(gl::quadConstantVertexData);

    namespace dtype = gl::ShaderAttributeType;
    gl::ShaderAttributeLayout quad_constant_layout = {
        {dtype::FLOAT(2), "acPos"},
        {dtype::FLOAT(2), "acUv" }
    };
    quad_constant_vbo.setLayout(quad_constant_layout);

    m_quadVao->bindVertexBuffer(std::move(quad_constant_vbo));
    m_quadVao->bindElementBuffer(gl::ElementBuffer(std::array<std::uint32_t, 6>{0, 1, 2, 2, 3, 0}));
}

void ImmediateQuadRenderPass::prepareQuadSsbo()
{
    m_quadSsbo = std::make_shared<gl::ShaderStorageBuffer<gl::QuadInstanceData>>(
        MAX_QUAD_COUNT * sizeof(gl::QuadInstanceData));
}

}  // namespace mono::renderer
