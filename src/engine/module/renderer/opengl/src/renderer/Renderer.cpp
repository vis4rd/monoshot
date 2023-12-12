#include "../../include/renderer/Renderer.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <spdlog/spdlog.h>

#include "../../include/shader/ShaderManager.hpp"
#include "resource/Resource.hpp"

namespace mono
{

Renderer& Renderer::get()
{
    static Renderer renderer;
    return renderer;
}

Renderer::Renderer()
{
    spdlog::debug("Renderer: creating OpenGL backend");

    //// Quads
    // data
    // TODO(vis4rd): use std::vector instead of c-style array
    // NOLINTNEXTLINE(modernize-avoid-c-arrays,cppcoreguidelines-avoid-c-arrays)
    std::uint32_t quad_elements[m_data.maxElementsCount];
    std::uint32_t offset = 0;
    for(std::size_t i = 0; i < m_data.maxElementsCount; i += 6)
    {
        quad_elements[i + 0] = 0 + offset;
        quad_elements[i + 1] = 1 + offset;
        quad_elements[i + 2] = 2 + offset;

        quad_elements[i + 3] = 2 + offset;
        quad_elements[i + 4] = 3 + offset;
        quad_elements[i + 5] = 0 + offset;

        offset += 4;
    }

    // buffers
    m_data.quadVao = std::make_shared<mono::gl::VertexArray>();
    auto quad_constant_vbo = mono::gl::VertexBuffer(
        reinterpret_cast<const float*>(mono::gl::quadConstantVertexData.data()),
        sizeof(mono::gl::quadConstantVertexData));
    auto quad_instance_vbo =
        mono::gl::VertexBuffer(m_data.maxQuadCount * sizeof(mono::gl::QuadInstanceData));
    auto quad_ebo = mono::gl::ElementBuffer(quad_elements, m_data.maxElementsCount);

    // attributes
    using dtype = mono::gl::ShaderDataType;
    using upd_freq = mono::gl::ShaderAttributeUpdateFrequency;

    mono::gl::ShaderAttributeLayout quad_constant_layout = {
        {dtype::FLOAT2, "acPos"},
        {dtype::FLOAT2, "acUv" }
    };
    quad_constant_vbo.setLayout(quad_constant_layout);

    mono::gl::ShaderAttributeLayout quad_instance_layout = {
        {dtype::FLOAT4, "aiColor",    upd_freq::EACH_INSTANCE},
        {dtype::FLOAT1, "aiTexIndex", upd_freq::EACH_INSTANCE},
        {dtype::MAT4,   "aiModel",    upd_freq::EACH_INSTANCE},
    };
    quad_instance_vbo.setLayout(quad_instance_layout);

    m_data.quadVao->addVertexBuffer(std::move(quad_constant_vbo));
    m_data.quadVao->addVertexBuffer(std::move(quad_instance_vbo));
    m_data.quadVao->addElementBuffer(quad_ebo);

    // shaders
    mono::gl::ShaderManager::get().addShaderProgram(
        "quad",
        "../res/shaders/quad.vert",
        "../res/shaders/quad.frag");

    //// Lines
    // buffers
    m_data.lineVao = std::make_shared<mono::gl::VertexArray>();
    auto line_vbo = mono::gl::VertexBuffer(m_data.maxVertexCount * sizeof(mono::gl::LineVertex));

    // attributes
    mono::gl::ShaderAttributeLayout line_layout = {
        {dtype::FLOAT3, "aPos"  },
        {dtype::FLOAT4, "aColor"},
    };
    line_vbo.setLayout(line_layout);

    m_data.lineVao->addVertexBuffer(std::move(line_vbo));

    // shaders
    mono::gl::ShaderManager::get().addShaderProgram(
        "line",
        "../res/shaders/line.vert",
        "../res/shaders/line.frag");


    // textures
    // iota: fill textureSamplers with 0, 1, 2, ..., 31
    std::iota(m_data.textureSamplers.begin(), m_data.textureSamplers.end(), 0);
    std::fill(m_data.textureFrameCounts.begin(), m_data.textureFrameCounts.end(), 1);
    std::fill(m_data.textureFrameRowLengths.begin(), m_data.textureFrameRowLengths.end(), 1);
    std::fill(m_data.textureFrameCurrentIndex.begin(), m_data.textureFrameCurrentIndex.end(), 0);

    const auto make_bytes = [](auto&&... args) -> std::array<std::byte, sizeof...(args)>
    {
        return {static_cast<std::byte>(std::forward<decltype(args)>(args))...};
    };

    const auto color = make_bytes(0xff, 0xff, 0xff, 0xff);
    std::shared_ptr<mono::Texture> texture = Resource::create<mono::Texture>(color.data(), 1, 1);

    m_data.textureSlots.reserve(32);
    m_data.textureSlots.push_back(texture);
    m_data.textureSlotsTakenCount++;
}

Renderer::~Renderer()
{
    m_data = mono::RendererData();
    spdlog::debug("Renderer: shutdown complete");
}

void Renderer::beginBatch()
{
    spdlog::trace(
        "Renderer: beginning a new batch, {} elements, {} quads, {} lines, {} instances",
        m_stats.indexCount,
        m_stats.quadCount,
        m_stats.lineCount,
        m_stats.instanceCount);
    m_stats.indexCount = 0;
    m_stats.quadCount = 0;
    m_stats.lineCount = 0;
    m_stats.instanceCount = 0;
    m_data.lineBufferIter = m_data.lineBuffer.begin();
    m_data.quadInstanceBuffer.clear();
}

void Renderer::endBatch(const glm::mat4& projection, const glm::mat4& view)
{
    m_data.lastProjectionMatrix = projection;
    m_data.lastViewMatrix = view;

    // quads
    if(m_stats.indexCount > 0)  // TODO(vis4rd): possibly can be removed in favor of just quadCount
    {
        // NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
        const GLsizeiptr instance_size = static_cast<std::uint32_t>(
            m_data.quadInstanceBuffer.size() * sizeof(mono::gl::QuadInstanceData));

        m_data.quadVao->getVertexBuffers().at(1).setData(
            reinterpret_cast<const void*>(m_data.quadInstanceBuffer.data()),
            instance_size);
        // NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)

        for(std::size_t slot = 0; slot < m_data.textureSlots.size(); slot++)
        {
            const auto& id = m_data.textureSlots[slot]->getID();
            glBindTextureUnit(slot, id);  // slot = unit
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        auto& quad_shader = mono::gl::ShaderManager::get().useShader("quad");
        quad_shader.uploadUniform("uProjection", projection, 0);
        quad_shader.uploadUniform("uView", view, 1);
        quad_shader.uploadUniform("uTextures", m_data.textureSamplers, 2);
        quad_shader.uploadUniform("uFrameCount", m_data.textureFrameCounts, 34);
        quad_shader.uploadUniform("uFrameRowLength", m_data.textureFrameRowLengths, 66);
        quad_shader.uploadUniform("uFrameCurrentIndex", m_data.textureFrameCurrentIndex, 98);

        m_data.quadVao->bind();
        glDrawElementsInstanced(
            GL_TRIANGLES,
            m_stats.indexCount,
            GL_UNSIGNED_INT,
            nullptr,
            m_stats.instanceCount);
        m_data.quadVao->unbind();

        glDisable(GL_BLEND);

        for(std::size_t slot = 0; slot < m_data.textureSlots.size(); slot++)
        {
            glBindTextureUnit(slot, 0);
        }
    }

    // lines
    if(m_stats.lineCount > 0)
    {
        // NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
        GLsizeiptr size = static_cast<std::uint32_t>(
            reinterpret_cast<std::uint8_t*>(&*m_data.lineBufferIter)
            - reinterpret_cast<std::uint8_t*>(&*(m_data.lineBuffer.begin())));
        m_data.lineVao->getVertexBuffers().at(0).setData(
            reinterpret_cast<const void*>(m_data.lineBuffer.data()),
            size);
        // NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)

        auto& line_shader = mono::gl::ShaderManager::get().useShader("line");

        m_data.lineVao->bind();

        glDrawArrays(GL_LINES, 0, m_stats.lineCount * 2);

        line_shader.uploadUniform("uProjection", projection, 0);
        line_shader.uploadUniform("uView", view, 1);
    }

    m_stats.drawCount++;
}

void Renderer::drawQuad(
    const glm::vec2& position,
    const glm::vec2& size,
    const float& rotation,
    const glm::vec4& color)
{
    drawQuad(position, size, rotation, m_data.textureSlots.at(0), color);
}

void Renderer::drawQuad(
    const glm::vec2& position,
    const glm::vec2& size,
    const float& rotation,
    const std::shared_ptr<mono::Texture>& texture,
    const glm::vec4& color)
{
    // spdlog::trace(
    //     "Renderer: drawing a Quad, position = ({}, {}), size = ({}, {}), rotation = {}",
    //     position.x,
    //     position.y,
    //     size.x,
    //     size.y,
    //     rotation);

    if((m_stats.indexCount >= m_data.maxElementsCount)
       or (m_data.textureSlotsTakenCount >= m_data.maxTextures))
    {
        endBatch(m_data.lastProjectionMatrix, m_data.lastViewMatrix);
        beginBatch();
    }

    const auto identity = glm::identity<glm::mat4>();
    glm::mat4 model_matrix = glm::translate(identity, glm::vec3(position, 0.f))
                             * glm::rotate(identity, glm::radians(rotation), {0.f, 0.f, 1.f})
                             * glm::scale(identity, glm::vec3(size, 1.f));

    const auto find_slot = [&m_data = m_data](
                               const std::vector<std::shared_ptr<mono::Texture>>& slots,
                               const std::uint32_t& texture_id) -> std::int64_t {
        for(std::int64_t slot = 0; slot < m_data.textureSlots.size(); slot++)
        {
            if(m_data.textureSlots[slot]->getID() == texture_id)
            {
                return slot;
            }
        }
        return -1;
    };

    std::int64_t texture_slot = find_slot(m_data.textureSlots, texture->getID());

    if(texture_slot == -1)
    {
        texture_slot = m_data.textureSlotsTakenCount;
        m_data.textureSlots.push_back(texture);
        m_data.textureSlotsTakenCount++;
    }
    m_data.textureFrameCounts.at(texture_slot) = texture->getTextureData().numberOfSubs;
    m_data.textureFrameRowLengths.at(texture_slot) = texture->getTextureData().numberOfSubsInOneRow;
    m_data.textureFrameCurrentIndex.at(texture_slot) = texture->getTextureData().currentSub;

    m_data.quadInstanceBuffer.emplace_back(color, static_cast<float>(texture_slot), model_matrix);

    m_stats.indexCount += 6;
    m_stats.instanceCount++;
    m_stats.quadCount++;
}

void Renderer::drawLine(const glm::vec2& pos1, const glm::vec2& pos2, const glm::vec4& color)
{
    Renderer::drawLine(pos1, pos2, color, color);
}

void Renderer::drawLine(
    const glm::vec2& pos1,
    const glm::vec2& pos2,
    const glm::vec4& color1,
    const glm::vec4& color2)
{
    m_data.lineBufferIter->position = glm::vec3(pos1, 0.f);
    m_data.lineBufferIter->color = color1;
    m_data.lineBufferIter++;

    m_data.lineBufferIter->position = glm::vec3(pos2, 0.f);
    m_data.lineBufferIter->color = color2;
    m_data.lineBufferIter++;

    m_stats.lineCount++;
}

void Renderer::drawRect(
    const glm::vec2& ul,
    const glm::vec2& br,
    const glm::vec4& color)  // upper-left + bottom-right
{
    const auto ur = glm::vec2(br.x, ul.y);
    const auto bl = glm::vec2(ul.x, br.y);
    Renderer::drawRect(ul, ur, br, bl, color);
}

void Renderer::drawRect(
    const glm::vec2& ul,
    const glm::vec2& ur,
    const glm::vec2& br,
    const glm::vec2& bl,
    const glm::vec4& color)  // 4 corners
{
    Renderer::drawLine(bl, br, color);
    Renderer::drawLine(br, ur, color);
    Renderer::drawLine(ur, ul, color);
    Renderer::drawLine(ul, bl, color);
}

void Renderer::drawRect(
    const glm::vec2& center,
    const glm::vec2& size,
    const float& rotation,
    const glm::vec4& color)  // center and size
{
    glm::mat4 model_matrix =
        glm::translate(glm::identity<glm::mat4>(), glm::vec3(center, 0.f))
        * glm::rotate(glm::identity<glm::mat4>(), glm::radians(rotation), {0.f, 0.f, 1.f})
        * glm::scale(glm::identity<glm::mat4>(), glm::vec3(size, 1.f));
    const auto bl =
        glm::vec2(model_matrix * glm::vec4(mono::gl::quadConstantVertexData[0], 0.0, 1.0));
    const auto br =
        glm::vec2(model_matrix * glm::vec4(mono::gl::quadConstantVertexData[2], 0.0, 1.0));
    const auto ur =
        glm::vec2(model_matrix * glm::vec4(mono::gl::quadConstantVertexData[4], 0.0, 1.0));
    const auto ul =
        glm::vec2(model_matrix * glm::vec4(mono::gl::quadConstantVertexData[6], 0.0, 1.0));
    Renderer::drawRect(ul, ur, br, bl, color);
}

mono::RendererStats& Renderer::getStats()
{
    return m_stats;
}

void Renderer::resetStats()
{
    m_stats = mono::RendererStats();
}

mono::RendererData& Renderer::getData()
{
    return m_data;
}

}  // namespace mono
