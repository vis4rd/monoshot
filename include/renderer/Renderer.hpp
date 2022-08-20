#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <array>
#include <numeric>

#include "../map/Grid.hpp"

class Renderer
{
    public:
    static void init();
    static void shutdown();

    static void beginBatch();
    static void endBatch();

    static void drawQuad(const glm::vec2& position, const glm::vec2& size, const float& rotation, const glm::vec4& color);
    static void drawQuad(const glm::vec2& position, const glm::vec2& size, const float& rotation, const std::uint32_t& texture_id, const glm::vec4& color = {1.f, 1.f, 1.f, 1.f});

    static void drawGrid(const Grid& grid, const std::array<glm::vec4, 6 * 9>& colors);
    static void drawGrid(const Grid& grid, const std::array<std::uint32_t, 6 * 9>& texture_ids, const std::array<glm::vec4, 6 * 9>& colors = {glm::vec4{}});
    static void drawGrid(const std::initializer_list<std::uint8_t>& tiles, const std::array<std::uint32_t, 6 * 9>& texture_ids, const std::array<glm::vec4, 6 * 9>& colors = {glm::vec4{}});

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

        std::uint32_t quadVao = 0;
        std::uint32_t quadVbo = 0;
        std::uint32_t quadEbo = 0;

        std::uint32_t whiteTexture = 0;

        std::array<QuadVertex, maxVertexCount> quadBuffer{};
        std::array<QuadVertex, maxVertexCount>::iterator quadBufferIter{};
        std::array<std::uint32_t, maxTextures> textureSlots = {0};
        std::array<std::int32_t, maxTextures> textureSamplers = {0};
        std::uint32_t textureSlotsTakenCount = 0;

        Renderer::Stats stats;
    };

    static Stats& getStats();
    static constexpr void resetStats();

    private:
    static Data& getData();
};

void Renderer::drawGrid(const Grid& grid, const std::array<glm::vec4, 6 * 9>& colors)
{
    std::array<std::uint32_t, 6 * 9> texture_ids;
    std::fill(texture_ids.begin(), texture_ids.end(), Renderer::getData().whiteTexture);
    drawGrid(grid, texture_ids, colors);
}

void Renderer::drawGrid(const Grid& grid, const std::array<std::uint32_t, 6 * 9>& texture_ids, const std::array<glm::vec4, 6 * 9>& colors)
{
    // pre-calculate center of the grid
    std::size_t center_x = 6 / 2;
    std::size_t center_y = 9 / 2;

    for(std::size_t iter = 0; iter < 6 * 9; iter++)
    {
        auto x = iter % 6;
        auto y = iter / 6;

        auto x_diff = static_cast<float>(x) - center_x;
        auto y_diff = static_cast<float>(y) - center_y;

        glm::vec4 quad_color = {colors[iter].r * grid[y][x], colors[iter].g * grid[y][x], colors[iter].b * grid[y][x], 1.f};

        drawQuad({x_diff, -y_diff}, {1.f, 1.f}, 0.f, texture_ids[iter], quad_color);
    }
}

void Renderer::drawGrid(const std::initializer_list<std::uint8_t>& tiles, const std::array<std::uint32_t, 6 * 9>& texture_ids, const std::array<glm::vec4, 6 * 9>& colors)
{
    Grid grid(tiles);
    drawGrid(grid, texture_ids, colors);
}
