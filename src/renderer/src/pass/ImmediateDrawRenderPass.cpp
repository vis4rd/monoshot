#include "renderer/pass/ImmediateDrawRenderPass.hpp"

#include "mono/util/IndexOf.hpp"

namespace mono::renderer
{

ImmediateDrawRenderPass::ImmediateDrawRenderPass(
    std::shared_ptr<mono::RenderTarget> render_target,
    mono::gl::ShaderProgram& quad_shader,
    mono::gl::ShaderProgram& line_shader)
    : RenderPassInterface(std::move(render_target))
    , m_quadPass(quad_shader)
    , m_linePass(line_shader)
{
    this->prepareQuadPass();
    this->prepareLinePass();
}

void ImmediateDrawRenderPass::submitDraws()
{
    ::gl::glClear(::gl::GL_COLOR_BUFFER_BIT | ::gl::GL_STENCIL_BUFFER_BIT);
    m_quadPass.submitDraws(m_projection, m_view);
    m_linePass.submitDraws(m_projection, m_view);
}

void ImmediateDrawRenderPass::drawQuad(
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
        const auto slot = util::indexOf(m_quadPass.textures, [texture_id](const auto& texture) {
            return texture->getID() == texture_id;
        });
        if(not slot)
        {
            m_quadPass.textures.push_back(std::move(texture));
            return m_quadPass.textures.size() - 1;
        }
        return slot.value();
    }();

    constexpr auto modulo = [](float value, float mod) -> float {
        return std::fmod(std::fmod(value, mod) + mod, mod);
    };

    const gl::QuadInstanceData quad_instance_data{
        .color = color_uint,
        .position = position,
        .scale = size,
        .rotation_texIndex =
            gl::detail::RtiPacked{static_cast<glm::uint32>(modulo(rotation, 360.f)), texture_slot}
    };

    m_quadPass.instances.push_back(quad_instance_data);
}

void ImmediateDrawRenderPass::drawQuad(
    const glm::vec2& position,
    const glm::vec2& size,
    float rotation,
    const glm::vec4& color)
{
    static std::shared_ptr<Texture> white_texture = std::make_shared<Texture>(
        std::array<std::byte, 4>{
            std::byte{0xff},
            std::byte{0xff},
            std::byte{0xff},
            std::byte{0xff}},
        1,
        1);
    return this->drawQuad(position, size, rotation, white_texture, color);
}

void ImmediateDrawRenderPass::drawLine(
    const glm::vec2& pos1,
    const glm::vec2& pos2,
    const glm::vec4& color1,
    const glm::vec4& color2)
{
    auto vrtx1 = gl::LineVertex{.position = glm::vec3(pos1, 0.f), .color = color1};
    auto vrtx2 = gl::LineVertex{.position = glm::vec3(pos2, 0.f), .color = color2};

    m_linePass.lines.push_back(vrtx1);
    m_linePass.lines.push_back(vrtx2);
}

void ImmediateDrawRenderPass::drawLine(
    const glm::vec2& pos1,
    const glm::vec2& pos2,
    const glm::vec4& color)
{
    this->drawLine(pos1, pos2, color, color);
}

void ImmediateDrawRenderPass::prepareQuadPass()
{
    m_quadPass.prepareVao();
    m_quadPass.prepareSsbo();
}

void ImmediateDrawRenderPass::prepareLinePass()
{
    m_linePass.prepareVao();
}

void ImmediateDrawRenderPass::QuadPass::prepareVao()
{
    this->vao = std::make_shared<gl::VertexArray>();
    auto vbo = gl::VertexBuffer(gl::quadConstantVertexData);

    namespace dtype = gl::ShaderAttributeType;
    gl::ShaderAttributeLayout layout = {
        {dtype::FLOAT(2), "acPos"},
        {dtype::FLOAT(2), "acUv" }
    };
    vbo.setLayout(layout);

    this->vao->bindVertexBuffer(std::move(vbo));
    this->vao->bindElementBuffer(gl::ElementBuffer(std::array<std::uint32_t, 6>{0, 1, 2, 2, 3, 0}));
}

void ImmediateDrawRenderPass::QuadPass::prepareSsbo()
{
    this->ssbo = std::make_shared<gl::ShaderStorageBuffer<gl::QuadInstanceData>>(
        MAX_QUAD_COUNT * sizeof(gl::QuadInstanceData));
}

void ImmediateDrawRenderPass::QuadPass::submitDraws(
    const glm::mat4& projection,
    const glm::mat4& view)
{
    if(not this->instances.empty())
    {
        this->ssbo->setData(this->instances);

        for(std::size_t slot = 0; slot < this->textures.size(); slot++)
        {
            // BUG: CAN GO OUT OF BOUND IF MORE THAN 32 TEXTURES!
            const auto& texture = this->textures[slot];
            const auto& id = texture->getID();
            ::gl::glBindTextureUnit(slot, id);  // slot = unit
        }

        ::gl::glEnable(::gl::GL_BLEND);
        ::gl::glBlendFunc(::gl::GL_SRC_ALPHA, ::gl::GL_ONE_MINUS_SRC_ALPHA);

        this->ssbo->bind(0);

        this->shader.use();

        this->shader.uploadUniform("uProjection", projection, 0);
        this->shader.uploadUniform("uView", view, 1);

        constexpr std::array<std::int32_t, 32> samplers{0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
                                                        11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
                                                        22, 23, 24, 25, 26, 27, 28, 29, 30, 31};
        this->shader.uploadUniform("uTextures", samplers, 2);

        this->vao->bind();
        ::gl::glDrawElementsInstanced(
            ::gl::GL_TRIANGLES,
            6,
            ::gl::GL_UNSIGNED_INT,
            nullptr,
            static_cast<::gl::GLsizei>(this->instances.size()));
        this->vao->unbind();
        this->ssbo->unbind();

        ::gl::glDisable(::gl::GL_BLEND);

        for(std::size_t slot = 0; slot < this->textures.size(); slot++)
        {
            ::gl::glBindTextureUnit(slot, 0);
        }

        this->instances.clear();
    }
}

void ImmediateDrawRenderPass::LinePass::prepareVao()
{
    this->vao = std::make_shared<gl::VertexArray>();

    auto vbo = gl::VertexBuffer(
        static_cast<::gl::GLsizeiptr>(MAX_LINE_COUNT * 4 * sizeof(gl::LineVertex)));

    namespace dtype = gl::ShaderAttributeType;
    gl::ShaderAttributeLayout layout = {
        {dtype::FLOAT(3), "aPos"  },
        {dtype::FLOAT(4), "aColor"},
    };
    vbo.setLayout(layout);

    this->vao->bindVertexBuffer(std::move(vbo));
}

void ImmediateDrawRenderPass::LinePass::submitDraws(
    const glm::mat4& projection,
    const glm::mat4& view)
{
    if(this->lines.empty())
    {
        return;
    }

    this->vao->getVertexBuffers().at(0).setData(this->lines);

    this->shader.use();

    this->shader.uploadUniform("uProjection", projection, 0);
    this->shader.uploadUniform("uView", view, 1);

    this->vao->bind();
    ::gl::glDrawArrays(::gl::GL_LINES, 0, static_cast<::gl::GLsizei>(this->lines.size()));
    this->vao->unbind();

    this->lines.clear();
}

}  // namespace mono::renderer
