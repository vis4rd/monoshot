#include "../../include/renderer/Renderer.hpp"

#include "glm/ext/matrix_transform.hpp"
#include "../../include/shader/ShaderManager.hpp"
#include <spdlog/spdlog.h>

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

    // data
    std::uint32_t indices[s_data.maxIndexCount];
    std::uint32_t offset = 0;
    for(std::size_t i = 0; i < s_data.maxIndexCount; i += 6)
    {
        indices[i + 0] = 0 + offset;
        indices[i + 1] = 1 + offset;
        indices[i + 2] = 2 + offset;

        indices[i + 3] = 2 + offset;
        indices[i + 4] = 3 + offset;
        indices[i + 5] = 0 + offset;

        offset += 4;
    }

    std::iota(s_data.textureSamplers.begin(), s_data.textureSamplers.end(), 0);  // fill textureSamplers with 0, 1, 2, ..., 31

    // buffers
    glCreateVertexArrays(1, &s_data.quadVao);

    glCreateBuffers(1, &s_data.quadVbo);
    glNamedBufferData(s_data.quadVbo, s_data.maxVertexCount * sizeof(QuadVertex), nullptr, GL_DYNAMIC_DRAW);
    glVertexArrayVertexBuffer(s_data.quadVao, /*vbo index for this vao*/ 0, s_data.quadVbo, 0, sizeof(QuadVertex));

    glCreateBuffers(1, &s_data.quadEbo);
    glNamedBufferData(s_data.quadEbo, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexArrayElementBuffer(s_data.quadVao, s_data.quadEbo);

    // attributes
    glVertexArrayAttribFormat(s_data.quadVao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(QuadVertex, position));
    glVertexArrayAttribBinding(s_data.quadVao, 0, /*vbo index for this vao*/ 0);
    glEnableVertexArrayAttrib(s_data.quadVao, 0);

    glVertexArrayAttribFormat(s_data.quadVao, 1, 4, GL_FLOAT, GL_FALSE, offsetof(QuadVertex, color));
    glVertexArrayAttribBinding(s_data.quadVao, 1, /*vbo index for this vao*/ 0);
    glEnableVertexArrayAttrib(s_data.quadVao, 1);

    glVertexArrayAttribFormat(s_data.quadVao, 2, 2, GL_FLOAT, GL_FALSE, offsetof(QuadVertex, texCoords));
    glVertexArrayAttribBinding(s_data.quadVao, 2, /*vbo index for this vao*/ 0);
    glEnableVertexArrayAttrib(s_data.quadVao, 2);

    glVertexArrayAttribFormat(s_data.quadVao, 3, 1, GL_FLOAT, GL_FALSE, offsetof(QuadVertex, texIndex));
    glVertexArrayAttribBinding(s_data.quadVao, 3, /*vbo index for this vao*/ 0);
    glEnableVertexArrayAttrib(s_data.quadVao, 3);

    // shaders
    ShaderManager::addShaderProgram("../res/shaders", "quad");

    // textures
    // glGenTextures(1, &s_data.whiteTexture);  // this would also work, but...
    glCreateTextures(GL_TEXTURE_2D, 1, &s_data.whiteTexture);  // ... this is technically OpenGL 4.5+ DSA
    glBindTexture(GL_TEXTURE_2D, s_data.whiteTexture);  // but we have to bind the texture here anyway (glCreate* doesn't do that for some reason)
    glTextureParameteri(s_data.whiteTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(s_data.whiteTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(s_data.whiteTexture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(s_data.whiteTexture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    std::uint32_t color = 0xffffffff;
    glTextureStorage2D(s_data.whiteTexture, /*number of tex level(??)*/ 1, /* tex format*/ GL_RGBA8, /*width*/ 1, /*height*/ 1);
    glTextureSubImage2D(s_data.whiteTexture, /*mipmap level*/ 0, /*xoffset*/ 0, /*yoffset*/ 0, /*width*/ 1, /*height*/ 1, /*data format*/ GL_RGBA, GL_UNSIGNED_BYTE, &color);

    s_data.textureSlots[0] = s_data.whiteTexture;
    s_data.textureSlotsTakenCount++;
}

void Renderer::shutdown()
{
    spdlog::debug("Renderer: Deleting VAO, VBOs and textures");
    glDeleteVertexArrays(1, &s_data.quadVao);
    glDeleteBuffers(1, &s_data.quadVbo);
    glDeleteBuffers(1, &s_data.quadEbo);
    glDeleteTextures(1, &s_data.whiteTexture);

    s_data.quadBuffer.fill({});
    Renderer::m_isInit = false;
    spdlog::debug("Renderer shutdown complete");
}

void Renderer::beginBatch()
{
    spdlog::trace("Renderer: beggining a new batch, index_count = {}", s_data.stats.indexCount);
    s_data.stats.indexCount = 0;
    s_data.quadBufferIter = s_data.quadBuffer.begin();
}

void Renderer::endBatch()
{
    if(s_data.stats.indexCount > 0)
    {
        spdlog::trace("Renderer: ending a batch");

        spdlog::trace("Renderer: filling up VB, sending data to gpu");
        GLsizeiptr size = static_cast<std::uint32_t>(reinterpret_cast<std::uint8_t*>(s_data.quadBufferIter) - reinterpret_cast<std::uint8_t*>(s_data.quadBuffer.begin()));
        glNamedBufferSubData(s_data.quadVbo, 0, size, reinterpret_cast<const void*>(s_data.quadBuffer.data()));

        spdlog::trace("Renderer: binding texture units");
        spdlog::trace("slots taken: {}", s_data.textureSlotsTakenCount);
        for(std::uint32_t i = 0; i < s_data.textureSlotsTakenCount; i++)
        {
            glBindTextureUnit(i, s_data.textureSlots[i]);
        }

        spdlog::trace("Renderer: binding shader 'quad'");
        auto& quad_shader = ShaderManager::useShader("quad");
        spdlog::trace("Renderer: uploading texture samplers");
        quad_shader.uploadArrayInt("uTextures", 32, s_data.textureSamplers.data(), 2);

        spdlog::trace("Renderer: binding VAO");
        glBindVertexArray(s_data.quadVao);
        glDrawElements(GL_TRIANGLES, s_data.stats.indexCount, GL_UNSIGNED_INT, nullptr);

        for(std::uint32_t i = 0; i < s_data.textureSlotsTakenCount; i++)
        {
            glBindTextureUnit(i, 0);
        }

        s_data.stats.drawCount++;
    }
}

void Renderer::drawQuad(const glm::vec2& position, const glm::vec2& size, const float& rotation, const glm::vec4& color)
{
    drawQuad(position, size, rotation, s_data.whiteTexture, color);
}

void Renderer::drawQuad(const glm::vec2& position, const glm::vec2& size, const float& rotation, const std::uint32_t& texture_id, const glm::vec4& color)
{
    spdlog::trace("Renderer: drawing a Quad, position = ({}, {}), size = ({}, {}), rotation = {}", position.x, position.y, size.x, size.y, rotation);

    if(s_data.stats.indexCount >= s_data.maxIndexCount || s_data.textureSlotsTakenCount >= s_data.textureSlots.size())
    {
        endBatch();
        beginBatch();
    }

    glm::mat4 model_matrix = glm::translate(glm::identity<glm::mat4>(), glm::vec3(position, 0.f)) * glm::rotate(glm::identity<glm::mat4>(), glm::radians(rotation), {0.f, 0.f, 1.f}) * glm::scale(glm::identity<glm::mat4>(), glm::vec3(size, 1.f));
    spdlog::trace("Renderer: model_matrix:\n{}", util::mat4str(model_matrix));
    float texture_index = -1.f;

    // TODO: consider changing this linear complexity to std::unordered_set
    for(std::uint32_t i = 0; i < s_data.textureSlotsTakenCount; i++)
    {
        if(s_data.textureSlots[i] == texture_id)
        {
            texture_index = static_cast<float>(i);
            break;
        }
    }
    //

    if(texture_index == -1.f)
    {
        texture_index = static_cast<float>(s_data.textureSlotsTakenCount);
        s_data.textureSlots[s_data.textureSlotsTakenCount] = texture_id;
        s_data.textureSlotsTakenCount++;
    }

    for(std::size_t i = 0; i < 4; i++)
    {
        s_data.quadBufferIter->position = model_matrix * quadVertexPositions[i];
        s_data.quadBufferIter->color = color;
        s_data.quadBufferIter->texCoords = quadTexturePositions[i];
        s_data.quadBufferIter->texIndex = texture_index;
        s_data.quadBufferIter++;
        spdlog::trace("Renderer: quad vertex {}: position = {}, color = {}, texCoords = {}, texIndex = {}",
            i,
            util::vec3str(s_data.quadBufferIter->position),
            util::vec4str(s_data.quadBufferIter->color),
            util::vec2str(s_data.quadBufferIter->texCoords),
            s_data.quadBufferIter->texIndex);
    }

    s_data.stats.indexCount += 6;
    s_data.stats.quadCount++;
}

Renderer::Stats& Renderer::getStats()
{
    static Renderer::Stats stats;
    return stats;
}

constexpr void Renderer::resetStats() { }

Renderer::Data& Renderer::getData()
{
    return s_data;
}
