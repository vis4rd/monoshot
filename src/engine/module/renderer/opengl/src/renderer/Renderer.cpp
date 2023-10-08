#include "../../include/renderer/Renderer.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <spdlog/spdlog.h>

#include "../../include/shader/ShaderManager.hpp"
#include "resource/Resource.hpp"

static constexpr std::array<glm::vec4, 4> quadVertexPositions = {
    glm::vec4{-0.5f, -0.5f, 0.f, 1.f},
    glm::vec4{0.5f,  -0.5f, 0.f, 1.f},
    glm::vec4{0.5f,  0.5f,  0.f, 1.f},
    glm::vec4{-0.5f, 0.5f,  0.f, 1.f}
};

static constexpr std::array<glm::vec2, 4> quadTexturePositions = {
    glm::vec2{0.0f, 0.0f},
    glm::vec2{1.0f, 0.0f},
    glm::vec2{1.0f, 1.0f},
    glm::vec2{0.0f, 1.0f}
};

Renderer::Renderer()
{
    if(m_isInit == true)
    {
        return;
    }
    m_isInit = true;

    spdlog::debug("Renderer: creating OpenGL backend");

    //// Quads
    // data
    std::uint32_t quadIndices[m_data.maxIndexCount];
    std::uint32_t offset = 0;
    for(std::size_t i = 0; i < m_data.maxIndexCount; i += 6)
    {
        quadIndices[i + 0] = 0 + offset;
        quadIndices[i + 1] = 1 + offset;
        quadIndices[i + 2] = 2 + offset;

        quadIndices[i + 3] = 2 + offset;
        quadIndices[i + 4] = 3 + offset;
        quadIndices[i + 5] = 0 + offset;

        offset += 4;
    }

    // buffers
    m_data.quadVao = std::make_shared<mono::VertexArray>();
    auto quadVbo = mono::VertexBuffer(m_data.maxVertexCount * sizeof(QuadVertex));
    auto quadEbo = mono::ElementBuffer(quadIndices, m_data.maxIndexCount);

    // attributes
    mono::BufferLayout quadLayout = {
        {mono::ShaderDataType::float3, "aPos"      },
        {mono::ShaderDataType::float4, "aColor"    },
        {mono::ShaderDataType::float2, "aTexCoords"},
        {mono::ShaderDataType::float1, "aTexIndex" },
    };
    quadVbo.setLayout(quadLayout);

    m_data.quadVao->addVertexBuffer(std::move(quadVbo));
    m_data.quadVao->addElementBuffer(quadEbo);

    // shaders
    mono::gl::ShaderManager::get().addShaderProgram(
        "quad",
        "../res/shaders/quad.vert",
        "../res/shaders/quad.frag");

    //// Lines
    // buffers
    m_data.lineVao = std::make_shared<mono::VertexArray>();
    auto lineVbo = mono::VertexBuffer(m_data.maxVertexCount * sizeof(LineVertex));

    // attributes
    mono::BufferLayout lineLayout = {
        {mono::ShaderDataType::float3, "aPos"  },
        {mono::ShaderDataType::float4, "aColor"},
    };
    lineVbo.setLayout(lineLayout);

    m_data.lineVao->addVertexBuffer(std::move(lineVbo));

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
        return {std::byte(std::forward<decltype(args)>(args))...};
    };

    const auto color = make_bytes(0xff, 0xff, 0xff, 0xff);
    Texture::Texture texture = Resource::create<Texture::impl::Texture>(color.data(), 1, 1);

    m_data.textureSlots.reserve(32);
    m_data.textureSlots.push_back(texture);
    m_data.textureSlotsTakenCount++;
}

Renderer::~Renderer()
{
    if(!m_isInit)
    {
        return;
    }
    spdlog::debug("Renderer: shutting down...");

    // spdlog::debug("Renderer: deleting quadBuffer");
    // s_data.quadBuffer.fill({});
    // spdlog::debug("Renderer: deleting lineBuffer");
    // s_data.lineBuffer.fill({});
    spdlog::debug("Renderer: deleting all RendererData");
    m_data = RendererData();
    Renderer::m_isInit = false;
    spdlog::debug("Renderer: shutdown complete");
}

void Renderer::beginBatch()
{
    spdlog::trace("Renderer: beginning a new batch, index_count = {}", m_stats.indexCount);
    m_stats.indexCount = 0;
    m_stats.quadCount = 0;
    m_stats.lineCount = 0;
    m_data.quadBufferIter = m_data.quadBuffer.begin();
    m_data.lineBufferIter = m_data.lineBuffer.begin();
}

void Renderer::endBatch(const glm::mat4& projection, const glm::mat4& view)
{
    // spdlog::trace("Renderer: ending a batch");  // commented for performance reasons
    m_data.last_projection_matrix = projection;
    m_data.last_view_matrix = view;

    // quads
    if(m_stats.indexCount > 0)  // TODO: possibly can be removed in favor of just quadCount
    {
        // spdlog::trace("Renderer: filling up VB, sending data to gpu");  // commented for
        // performance reasons
        GLsizeiptr size = static_cast<std::uint32_t>(
            reinterpret_cast<std::uint8_t*>(&*m_data.quadBufferIter)
            - reinterpret_cast<std::uint8_t*>(&*(m_data.quadBuffer.begin())));
        m_data.quadVao->getVertexBuffers().at(0).setData(
            reinterpret_cast<const void*>(m_data.quadBuffer.data()),
            size);

        // spdlog::trace("Renderer: binding texture IDs");  // commented for performance reasons
        // spdlog::trace("slots taken: {}", m_data.textureSlotsTakenCount);  // commented for
        // performance reasons

        for(std::size_t slot = 0; slot < m_data.textureSlots.size(); slot++)
        {
            const auto& id = m_data.textureSlots[slot]->getID();
            // spdlog::trace("Binding texture slot {} to unit {}", slot, id);  // commented for
            // performance reasons
            glBindTextureUnit(slot, id);  // slot = unit
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // spdlog::trace("Renderer: binding shader 'quad'");  // commented for performance reasons
        auto& quad_shader = mono::gl::ShaderManager::get().useShader("quad");

        // spdlog::trace("Renderer: binding quad VAO");  // commented for performance reasons
        m_data.quadVao->bind();
        glDrawElements(GL_TRIANGLES, m_stats.indexCount, GL_UNSIGNED_INT, nullptr);

        // spdlog::trace("Renderer: uploading uniforms");  // commented for performance reasons
        quad_shader.uploadArrayInt(
            "uTextures",
            m_data.textureSlotsTakenCount,
            m_data.textureSamplers.data(),
            2);
        quad_shader.uploadMat4("uProjection", projection, 0);
        quad_shader.uploadMat4("uView", view, 1);
        quad_shader.uploadArrayUInt(
            "uFrameCount",
            m_data.textureSlotsTakenCount,
            m_data.textureFrameCounts.data(),
            34);
        quad_shader.uploadArrayUInt(
            "uFrameRowLength",
            m_data.textureSlotsTakenCount,
            m_data.textureFrameRowLengths.data(),
            66);
        quad_shader.uploadArrayUInt(
            "uFrameCurrentIndex",
            m_data.textureSlotsTakenCount,
            m_data.textureFrameCurrentIndex.data(),
            98);

        glDisable(GL_BLEND);

        // spdlog::trace("Renderer: unbinding texture IDs from slots");  // commented for
        // performance reasons
        for(std::size_t slot = 0; slot < m_data.textureSlots.size(); slot++)
        {
            glBindTextureUnit(slot, 0);
        }
    }

    // lines
    if(m_stats.lineCount > 0)
    {
        // spdlog::trace("Renderer: drawing lines");  // commented for performance reasons
        GLsizeiptr size = static_cast<std::uint32_t>(
            reinterpret_cast<std::uint8_t*>(&*m_data.lineBufferIter)
            - reinterpret_cast<std::uint8_t*>(&*(m_data.lineBuffer.begin())));
        m_data.lineVao->getVertexBuffers().at(0).setData(
            reinterpret_cast<const void*>(m_data.lineBuffer.data()),
            size);

        // spdlog::trace("Renderer: binding shader 'line'");  // commented for performance reasons
        auto& line_shader = mono::gl::ShaderManager::get().useShader("line");

        // spdlog::trace("Renderer: binding line VAO");  // commented for performance reasons
        m_data.lineVao->bind();

        // spdlog::trace("Renderer: drawing line arrays");  // commented for performance reasons
        glDrawArrays(GL_LINES, 0, m_stats.lineCount * 2);

        // spdlog::trace("Renderer: uploading uniforms");  // commented for performance reasons
        line_shader.uploadMat4("uProjection", projection, 0);
        line_shader.uploadMat4("uView", view, 1);
    }

    m_stats.drawCount++;
    // spdlog::trace("Renderer: finished batch");  // commented for performance reasons
}

void Renderer::drawQuad(
    const glm::vec2& position,
    const glm::vec2& size,
    const float& rotation,
    const glm::vec4& color)
{
    drawQuad(position, size, rotation, m_data.textureSlots.at(0), color);
}

// static float findSlot(const std::vector<Texture::Texture>& slots, const std::uint32_t&
// texture_id)
// {
//     for(std::size_t slot = 0; slot < m_data.textureSlots.size(); slot++)
//     {
//         if(m_data.textureSlots[slot]->getID() == texture_id)
//         {
//             return static_cast<float>(slot);
//         }
//     }
//     return -1.f;
// }

void Renderer::drawQuad(
    const glm::vec2& position,
    const glm::vec2& size,
    const float& rotation,
    const Texture::Texture& texture,
    const glm::vec4& color)
{
    // spdlog::trace("Renderer: drawing a Quad, position = ({}, {}), size = ({}, {}), rotation =
    // {}", position.x, position.y, size.x, size.y, rotation);

    if(m_stats.indexCount >= m_data.maxIndexCount
       || m_data.textureSlotsTakenCount >= m_data.maxTextures)
    {
        endBatch(m_data.last_projection_matrix, m_data.last_view_matrix);
        beginBatch();
    }

    const auto identity = glm::identity<glm::mat4>();
    glm::mat4 model_matrix = glm::translate(identity, glm::vec3(position, 0.f))
                             * glm::rotate(identity, glm::radians(rotation), {0.f, 0.f, 1.f})
                             * glm::scale(identity, glm::vec3(size, 1.f));
    // spdlog::trace("Renderer: model_matrix:\n{}", util::mat4str(model_matrix));

    auto find_slot = [&m_data = m_data](
                         const std::vector<Texture::Texture>& slots,
                         const std::uint32_t& texture_id) -> float {
        for(std::size_t slot = 0; slot < m_data.textureSlots.size(); slot++)
        {
            if(m_data.textureSlots[slot]->getID() == texture_id)
            {
                return static_cast<float>(slot);
            }
        }
        return -1.f;
    };

    float texture_slot = find_slot(m_data.textureSlots, texture->getID());
    // spdlog::trace("texture_slot = {} of texture_id {}", texture_slot, texture->getID());

    if(texture_slot == -1.f)
    {
        texture_slot = static_cast<float>(m_data.textureSlotsTakenCount);
        m_data.textureSlots.push_back(texture);
        m_data.textureSlotsTakenCount++;
    }
    m_data.textureFrameCounts[texture_slot] = texture->getTextureData().numberOfSubs;
    m_data.textureFrameRowLengths[texture_slot] = texture->getTextureData().numberOfSubsInOneRow;
    m_data.textureFrameCurrentIndex[texture_slot] = texture->getTextureData().currentSub;

    for(std::size_t i = 0; i < 4; i++)
    {
        m_data.quadBufferIter->position = model_matrix * quadVertexPositions[i];
        m_data.quadBufferIter->color = color;
        m_data.quadBufferIter->texCoords = quadTexturePositions[i];
        m_data.quadBufferIter->texIndex = texture_slot;
        m_data.quadBufferIter++;
        /*spdlog::trace("Renderer: quad vertex {}: position = {}, color = {}, texCoords = {},
           texIndex = {}", i, util::vec3str(m_data.quadBufferIter->position),
            util::vec4str(m_data.quadBufferIter->color),
            util::vec2str(m_data.quadBufferIter->texCoords),
            m_data.quadBufferIter->texIndex);*/
    }

    m_stats.indexCount += 6;
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
    const auto bl = glm::vec2(model_matrix * quadVertexPositions[0]);
    const auto br = glm::vec2(model_matrix * quadVertexPositions[1]);
    const auto ur = glm::vec2(model_matrix * quadVertexPositions[2]);
    const auto ul = glm::vec2(model_matrix * quadVertexPositions[3]);
    Renderer::drawRect(ul, ur, br, bl, color);
}

RendererStats& Renderer::getStats()
{
    return m_stats;
}

void Renderer::resetStats()
{
    m_stats = RendererStats();
}

RendererData& Renderer::getData()
{
    return m_data;
}
