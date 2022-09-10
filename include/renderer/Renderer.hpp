#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <array>
#include <numeric>
#include <unordered_set>
#include <unordered_map>
#include <memory>

#include "../utility/VertexArray.hpp"

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
    static void drawQuad(const glm::vec2& position, const glm::vec2& size, const float& rotation, const std::uint32_t& texture_id, const glm::vec4& color = {1.f, 1.f, 1.f, 1.f});

    struct Stats
    {
        std::uint32_t drawCount = 0;
        std::uint32_t quadCount = 0;
        std::uint32_t indexCount = 0;
    };

    private:
    struct QuadVertex
    {
        glm::vec3 position;
        glm::vec4 color;
        glm::vec2 texCoords;
        float texIndex;
    };

    public:
    struct Data
    {
        static constinit const std::size_t maxQuadCount = 10000;
        static constinit const std::size_t maxVertexCount = maxQuadCount * 4;
        static constinit const std::size_t maxIndexCount = maxQuadCount * 6;
        static constinit const std::size_t maxTextures = 32;

        // std::uint32_t quadVao = 0;
        // std::uint32_t quadVbo = 0;
        // std::uint32_t quadEbo = 0;
        ref<VertexArray> quadVao;

        std::uint32_t whiteTexture = 0;

        std::array<QuadVertex, maxVertexCount> quadBuffer{};
        std::array<QuadVertex, maxVertexCount>::iterator quadBufferIter{};
        std::unordered_map<std::uint32_t, std::uint32_t> textureSlots;  // slot - unit
        std::unordered_map<std::uint32_t, std::uint32_t> usedTextureSlots;  // slot - unit
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
