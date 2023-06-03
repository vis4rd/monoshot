#include "../../include/renderer/Renderer.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <spdlog/spdlog.h>

#include "../../include/shader/ShaderManager.hpp"
#include "resource/Resource.hpp"

static Renderer::Data s_data;
bool Renderer::m_isInit = false;

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

void Renderer::init()
{
    if(Renderer::m_isInit == true)
    {
        return;
    }
    Renderer::m_isInit = true;

    spdlog::debug("Renderer: creating OpenGL backend");

    //// Quads
    // data
    std::uint32_t quadIndices[s_data.maxIndexCount];
    std::uint32_t offset = 0;
    for(std::size_t i = 0; i < s_data.maxIndexCount; i += 6)
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
    s_data.quadVao = std::make_shared<VertexArray>();
    auto quadVbo = VertexBuffer(s_data.maxVertexCount * sizeof(QuadVertex));
    auto quadEbo = ElementBuffer(quadIndices, s_data.maxIndexCount);

    // attributes
    BufferLayout quadLayout = {
        BufferElement(ShaderDataType::float3, "aPos"),
        BufferElement(ShaderDataType::float4, "aColor"),
        BufferElement(ShaderDataType::float2, "aTexCoords"),
        BufferElement(ShaderDataType::float1, "aTexIndex"),
    };
    quadVbo.setLayout(quadLayout);

    s_data.quadVao->addVertexBuffer(std::move(quadVbo));
    s_data.quadVao->addElementBuffer(quadEbo);

    // shaders
    ShaderManager::addShaderProgram("../res/shaders", "quad");

    //// Lines
    // buffers
    s_data.lineVao = std::make_shared<VertexArray>();
    auto lineVbo = VertexBuffer(s_data.maxVertexCount * sizeof(LineVertex));

    // attributes
    BufferLayout lineLayout = {
        BufferElement(ShaderDataType::float3, "aPos"),
        BufferElement(ShaderDataType::float4, "aColor"),
    };
    lineVbo.setLayout(lineLayout);

    s_data.lineVao->addVertexBuffer(std::move(lineVbo));

    // shaders
    ShaderManager::addShaderProgram("../res/shaders", "line");


    // textures
    std::iota(s_data.textureSamplers.begin(),
        s_data.textureSamplers.end(),
        0);  // fill textureSamplers with 0, 1, 2, ..., 31
    std::fill(s_data.textureFrameCounts.begin(), s_data.textureFrameCounts.end(), 1);
    std::fill(s_data.textureFrameRowLengths.begin(), s_data.textureFrameRowLengths.end(), 1);
    std::fill(s_data.textureFrameCurrentIndex.begin(), s_data.textureFrameCurrentIndex.end(), 0);

    const auto make_bytes = [](auto&&... args) -> std::array<std::byte, sizeof...(args)>
    {
        return {std::byte(std::forward<decltype(args)>(args))...};
    };

    const auto color = make_bytes(0xff, 0xff, 0xff, 0xff);
    Texture::Texture texture = Resource::create<Texture::impl::Texture>(color.data(), 1, 1);

    s_data.textureSlots.reserve(32);
    s_data.textureSlots.push_back(texture);
    s_data.textureSlotsTakenCount++;
}

void Renderer::shutdown()
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
    s_data = Renderer::Data();
    Renderer::m_isInit = false;
    spdlog::debug("Renderer: shutdown complete");
}

void Renderer::beginBatch()
{
    spdlog::trace("Renderer: beginning a new batch, index_count = {}", s_data.stats.indexCount);
    s_data.stats.indexCount = 0;
    s_data.stats.quadCount = 0;
    s_data.stats.lineCount = 0;
    s_data.quadBufferIter = s_data.quadBuffer.begin();
    s_data.lineBufferIter = s_data.lineBuffer.begin();
}

void Renderer::endBatch(const glm::mat4& projection, const glm::mat4& view)
{
    // spdlog::trace("Renderer: ending a batch");  // commented for performance reasons
    s_data.last_projection_matrix = projection;
    s_data.last_view_matrix = view;

    // quads
    if(s_data.stats.indexCount > 0)  // TODO: possibly can be removed in favor of just quadCount
    {
        // spdlog::trace("Renderer: filling up VB, sending data to gpu");  // commented for
        // performance reasons
        GLsizeiptr size = static_cast<std::uint32_t>(
            reinterpret_cast<std::uint8_t*>(&*s_data.quadBufferIter)
            - reinterpret_cast<std::uint8_t*>(&*(s_data.quadBuffer.begin())));
        s_data.quadVao->getVertexBuffers().at(0).setData(
            reinterpret_cast<const void*>(s_data.quadBuffer.data()),
            size);

        // spdlog::trace("Renderer: binding texture IDs");  // commented for performance reasons
        // spdlog::trace("slots taken: {}", s_data.textureSlotsTakenCount);  // commented for
        // performance reasons

        for(std::size_t slot = 0; slot < s_data.textureSlots.size(); slot++)
        {
            const auto& id = s_data.textureSlots[slot]->getID();
            // spdlog::trace("Binding texture slot {} to unit {}", slot, id);  // commented for
            // performance reasons
            glBindTextureUnit(slot, id);  // slot = unit
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // spdlog::trace("Renderer: binding shader 'quad'");  // commented for performance reasons
        auto& quad_shader = ShaderManager::useShader("quad");

        // spdlog::trace("Renderer: binding quad VAO");  // commented for performance reasons
        s_data.quadVao->bind();
        glDrawElements(GL_TRIANGLES, s_data.stats.indexCount, GL_UNSIGNED_INT, nullptr);

        // spdlog::trace("Renderer: uploading uniforms");  // commented for performance reasons
        quad_shader.uploadArrayInt("uTextures",
            s_data.textureSlotsTakenCount,
            s_data.textureSamplers.data(),
            2);
        quad_shader.uploadMat4("uProjection", projection, 0);
        quad_shader.uploadMat4("uView", view, 1);
        quad_shader.uploadArrayUInt("uFrameCount",
            s_data.textureSlotsTakenCount,
            s_data.textureFrameCounts.data(),
            34);
        quad_shader.uploadArrayUInt("uFrameRowLength",
            s_data.textureSlotsTakenCount,
            s_data.textureFrameRowLengths.data(),
            66);
        quad_shader.uploadArrayUInt("uFrameCurrentIndex",
            s_data.textureSlotsTakenCount,
            s_data.textureFrameCurrentIndex.data(),
            98);

        glDisable(GL_BLEND);

        // spdlog::trace("Renderer: unbinding texture IDs from slots");  // commented for
        // performance reasons
        for(std::size_t slot = 0; slot < s_data.textureSlots.size(); slot++)
        {
            glBindTextureUnit(slot, 0);
        }
    }

    // lines
    if(s_data.stats.lineCount > 0)
    {
        // spdlog::trace("Renderer: drawing lines");  // commented for performance reasons
        GLsizeiptr size = static_cast<std::uint32_t>(
            reinterpret_cast<std::uint8_t*>(&*s_data.lineBufferIter)
            - reinterpret_cast<std::uint8_t*>(&*(s_data.lineBuffer.begin())));
        s_data.lineVao->getVertexBuffers().at(0).setData(
            reinterpret_cast<const void*>(s_data.lineBuffer.data()),
            size);

        // spdlog::trace("Renderer: binding shader 'line'");  // commented for performance reasons
        auto& line_shader = ShaderManager::useShader("line");

        // spdlog::trace("Renderer: binding line VAO");  // commented for performance reasons
        s_data.lineVao->bind();

        // spdlog::trace("Renderer: drawing line arrays");  // commented for performance reasons
        glDrawArrays(GL_LINES, 0, s_data.stats.lineCount * 2);

        // spdlog::trace("Renderer: uploading uniforms");  // commented for performance reasons
        line_shader.uploadMat4("uProjection", projection, 0);
        line_shader.uploadMat4("uView", view, 1);
    }

    s_data.stats.drawCount++;
    // spdlog::trace("Renderer: finished batch");  // commented for performance reasons
}

void Renderer::drawQuad(const glm::vec2& position,
    const glm::vec2& size,
    const float& rotation,
    const glm::vec4& color)
{
    drawQuad(position, size, rotation, s_data.textureSlots.at(0), color);
}

// static float findSlot(const std::vector<Texture::Texture>& slots, const std::uint32_t&
// texture_id)
// {
//     for(std::size_t slot = 0; slot < s_data.textureSlots.size(); slot++)
//     {
//         if(s_data.textureSlots[slot]->getID() == texture_id)
//         {
//             return static_cast<float>(slot);
//         }
//     }
//     return -1.f;
// }

void Renderer::drawQuad(const glm::vec2& position,
    const glm::vec2& size,
    const float& rotation,
    const Texture::Texture& texture,
    const glm::vec4& color)
{
    // spdlog::trace("Renderer: drawing a Quad, position = ({}, {}), size = ({}, {}), rotation =
    // {}", position.x, position.y, size.x, size.y, rotation);

    if(s_data.stats.indexCount >= s_data.maxIndexCount
        || s_data.textureSlotsTakenCount >= s_data.maxTextures)
    {
        endBatch(s_data.last_projection_matrix, s_data.last_view_matrix);
        beginBatch();
    }

    const auto identity = glm::identity<glm::mat4>();
    glm::mat4 model_matrix = glm::translate(identity, glm::vec3(position, 0.f))
                             * glm::rotate(identity, glm::radians(rotation), {0.f, 0.f, 1.f})
                             * glm::scale(identity, glm::vec3(size, 1.f));
    // spdlog::trace("Renderer: model_matrix:\n{}", util::mat4str(model_matrix));

    constexpr auto find_slot = [](const std::vector<Texture::Texture>& slots,
                                   const std::uint32_t& texture_id) -> float {
        for(std::size_t slot = 0; slot < s_data.textureSlots.size(); slot++)
        {
            if(s_data.textureSlots[slot]->getID() == texture_id)
            {
                return static_cast<float>(slot);
            }
        }
        return -1.f;
    };

    float texture_slot = find_slot(s_data.textureSlots, texture->getID());
    // spdlog::trace("texture_slot = {} of texture_id {}", texture_slot, texture->getID());

    if(texture_slot == -1.f)
    {
        texture_slot = static_cast<float>(s_data.textureSlotsTakenCount);
        s_data.textureSlots.push_back(texture);
        s_data.textureSlotsTakenCount++;
    }
    s_data.textureFrameCounts[texture_slot] = texture->getTextureData().numberOfSubs;
    s_data.textureFrameRowLengths[texture_slot] = texture->getTextureData().numberOfSubsInOneRow;
    s_data.textureFrameCurrentIndex[texture_slot] = texture->getTextureData().currentSub;

    for(std::size_t i = 0; i < 4; i++)
    {
        s_data.quadBufferIter->position = model_matrix * quadVertexPositions[i];
        s_data.quadBufferIter->color = color;
        s_data.quadBufferIter->texCoords = quadTexturePositions[i];
        s_data.quadBufferIter->texIndex = texture_slot;
        s_data.quadBufferIter++;
        /*spdlog::trace("Renderer: quad vertex {}: position = {}, color = {}, texCoords = {},
           texIndex = {}", i, util::vec3str(s_data.quadBufferIter->position),
            util::vec4str(s_data.quadBufferIter->color),
            util::vec2str(s_data.quadBufferIter->texCoords),
            s_data.quadBufferIter->texIndex);*/
    }

    s_data.stats.indexCount += 6;
    s_data.stats.quadCount++;
}

void Renderer::drawLine(const glm::vec2& pos1, const glm::vec2& pos2, const glm::vec4& color)
{
    Renderer::drawLine(pos1, pos2, color, color);
}

void Renderer::drawLine(const glm::vec2& pos1,
    const glm::vec2& pos2,
    const glm::vec4& color1,
    const glm::vec4& color2)
{
    s_data.lineBufferIter->position = glm::vec3(pos1, 0.f);
    s_data.lineBufferIter->color = color1;
    s_data.lineBufferIter++;

    s_data.lineBufferIter->position = glm::vec3(pos2, 0.f);
    s_data.lineBufferIter->color = color2;
    s_data.lineBufferIter++;

    s_data.stats.lineCount++;
}

void Renderer::drawRect(const glm::vec2& ul,
    const glm::vec2& br,
    const glm::vec4& color)  // upper-left + bottom-right
{
    const auto ur = glm::vec2(br.x, ul.y);
    const auto bl = glm::vec2(ul.x, br.y);
    Renderer::drawRect(ul, ur, br, bl, color);
}

void Renderer::drawRect(const glm::vec2& ul,
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

void Renderer::drawRect(const glm::vec2& center,
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

Renderer::Stats& Renderer::getStats()
{
    return s_data.stats;
}

constexpr void Renderer::resetStats() { }

Renderer::Data& Renderer::getData()
{
    return s_data;
}
