#pragma once

#include <array>
#include <memory>
#include <numeric>
#include <unordered_map>
#include <unordered_set>

#include <glm/glm.hpp>

#include "../gl/VertexArray.hpp"
#include "../texture/Texture.hpp"

class Renderer
{
    public:
    static void init();
    static void shutdown();

    static void beginBatch();
    static void endBatch(const glm::mat4& projection, const glm::mat4& view);

    static void drawQuad(const glm::vec2& position,
        const glm::vec2& size,
        const float& rotation,
        const glm::vec4& color);
    static void drawQuad(const glm::vec2& position,
        const glm::vec2& size,
        const float& rotation,
        const Texture::Texture& texture,
        const glm::vec4& color = {1.f, 1.f, 1.f, 1.f});

    static void drawLine(const glm::vec2& pos1, const glm::vec2& pos2, const glm::vec4& color);
    static void drawLine(const glm::vec2& pos1,
        const glm::vec2& pos2,
        const glm::vec4& color1,
        const glm::vec4& color2);

    static void drawRect(const glm::vec2& ul,
        const glm::vec2& br,
        const glm::vec4& color);  // upper-left + bottom-right corners
    static void drawRect(const glm::vec2& ul,
        const glm::vec2& ur,
        const glm::vec2& br,
        const glm::vec2& bl,
        const glm::vec4& color);  // 4 corners
    static void drawRect(const glm::vec2& center,
        const glm::vec2& size,
        const float& rotation,
        const glm::vec4& color);  // center and size

    struct Stats
    {
        std::uint32_t drawCount = 0;
        std::uint32_t indexCount = 0;

        std::uint32_t quadCount = 0;
        std::uint32_t lineCount = 0;
    };

    private:
    struct QuadVertex
    {
        glm::vec3 position;
        glm::vec4 color;
        glm::vec2 texCoords;
        float texIndex;
    };

    struct LineVertex
    {
        glm::vec3 position;
        glm::vec4 color;
    };

    public:
    struct Data
    {
        static constinit const std::size_t maxLineCount = 10000;
        static constinit const std::size_t maxQuadCount = 10000;
        static constinit const std::size_t maxVertexCount = maxQuadCount * 4;
        static constinit const std::size_t maxIndexCount = maxQuadCount * 6;
        static constinit const std::size_t maxTextures = 32;

        std::shared_ptr<mono::VertexArray> quadVao;
        std::vector<QuadVertex> quadBuffer = std::vector<QuadVertex>(maxVertexCount);
        std::vector<QuadVertex>::iterator quadBufferIter{};

        std::shared_ptr<mono::VertexArray> lineVao;
        std::vector<LineVertex> lineBuffer = std::vector<LineVertex>(maxLineCount * 2);
        std::vector<LineVertex>::iterator lineBufferIter{};

        std::vector<Texture::Texture> textureSlots;  // slot = vec index, unit = tex ID
        // TODO: if shaders can't stand almost 100 uniforms, yeet arrays below to an array of
        // structs...
        // TODO: ... and use this: https://www.khronos.org/opengl/wiki/Interface_Block_(GLSL)
        std::array<std::int32_t, maxTextures> textureSamplers = {0};
        std::array<std::uint32_t, maxTextures> textureFrameCounts = {1};
        std::array<std::uint32_t, maxTextures> textureFrameRowLengths = {1};
        std::array<std::uint32_t, maxTextures> textureFrameCurrentIndex = {0};
        std::uint32_t textureSlotsTakenCount = 0;

        glm::mat4 last_projection_matrix{};
        glm::mat4 last_view_matrix{};

        Renderer::Stats stats;
    };

    static Stats& getStats();
    static constexpr void resetStats();

    private:
    static Data& getData();

    private:
    static bool m_isInit;
};
