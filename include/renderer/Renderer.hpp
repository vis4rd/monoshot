#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <array>
#include <numeric>
#include <unordered_set>
#include <unordered_map>
#include <memory>

#include "../utility/VertexArray.hpp"
#include "../texture/Texture2D.hpp"

class Renderer
{
    public:
    template<typename T>
    using ref = std::shared_ptr<T>;

    private:
    template<typename T, typename... Args>
    static constexpr ref<T> make_ref(Args&&... args)
    {
        return std::make_shared<T>(args...);
    }

    public:
    static void init();
    static void shutdown();

    static void beginBatch();
    static void endBatch();

    static void drawQuad(const glm::vec2& position, const glm::vec2& size, const float& rotation, const glm::vec4& color);
    static void drawQuad(const glm::vec2& position, const glm::vec2& size, const float& rotation, const ref<Texture2D> texture, const glm::vec4& color = {1.f, 1.f, 1.f, 1.f});

    static void drawLine(const glm::vec2& pos1, const glm::vec2& pos2, const glm::vec4& color);

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

        ref<VertexArray> quadVao;
        std::vector<QuadVertex> quadBuffer = std::vector<QuadVertex>(maxVertexCount);
        std::vector<QuadVertex>::iterator quadBufferIter{};

        ref<VertexArray> lineVao;
        std::vector<LineVertex> lineBuffer = std::vector<LineVertex>(maxLineCount * 2);
        std::vector<LineVertex>::iterator lineBufferIter{};

        std::vector<ref<Texture2D>> textureSlots;  // slot = vec index, unit = tex ID
        std::array<std::int32_t, maxTextures> textureSamplers = {0};
        std::uint32_t textureSlotsTakenCount = 0;

        Renderer::Stats stats;
    };

    static Stats& getStats();
    static constexpr void resetStats();

    private:
    static Data& getData();

    private:
    static bool m_isInit;
};
