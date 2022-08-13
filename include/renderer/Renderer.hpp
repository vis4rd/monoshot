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

    template<std::size_t WIDTH, std::size_t HEIGHT>
    static void drawGrid(const Grid<WIDTH, HEIGHT>& grid, const std::array<glm::vec4, WIDTH * HEIGHT>& colors);
    template<std::size_t WIDTH, std::size_t HEIGHT>
    static void drawGrid(const Grid<WIDTH, HEIGHT>& grid, const std::array<std::uint32_t, WIDTH * HEIGHT>& texture_ids, const std::array<glm::vec4, WIDTH * HEIGHT>& colors = {glm::vec4{}});
    template<std::size_t WIDTH, std::size_t HEIGHT>
    static void drawGrid(const std::initializer_list<typename Grid<WIDTH, HEIGHT>::tile_t>& tiles, const std::array<std::uint32_t, WIDTH * HEIGHT>& texture_ids, const std::array<glm::vec4, WIDTH * HEIGHT>& colors = {glm::vec4{}});

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

template<std::size_t WIDTH, std::size_t HEIGHT>
void Renderer::drawGrid(const Grid<WIDTH, HEIGHT>& grid, const std::array<glm::vec4, WIDTH * HEIGHT>& colors)
{
    std::array<std::uint32_t, WIDTH * HEIGHT> texture_ids;
    std::fill(texture_ids.begin(), texture_ids.end(), Renderer::getData().whiteTexture);
    drawGrid(grid, texture_ids, colors);
}

template<std::size_t WIDTH, std::size_t HEIGHT>
void Renderer::drawGrid(const Grid<WIDTH, HEIGHT>& grid, const std::array<std::uint32_t, WIDTH * HEIGHT>& texture_ids, const std::array<glm::vec4, WIDTH * HEIGHT>& colors)
{
    // pre-calculate center of the grid
    std::size_t center_x = WIDTH / 2;
    std::size_t center_y = HEIGHT / 2;

    for(std::size_t iter = 0; iter < WIDTH * HEIGHT; iter++)
    {
        auto x = iter % WIDTH;
        auto y = iter / WIDTH;

        auto x_diff = static_cast<float>(x) - center_x;
        auto y_diff = static_cast<float>(y) - center_y;

        glm::vec4 quad_color = {colors[iter].r * grid[y][x], colors[iter].g * grid[y][x], colors[iter].b * grid[y][x], 1.f};

        drawQuad({x_diff, -y_diff}, {1.f, 1.f}, 0.f, texture_ids[iter], quad_color);
    }
}

template<std::size_t WIDTH, std::size_t HEIGHT>
void Renderer::drawGrid(const std::initializer_list<typename Grid<WIDTH, HEIGHT>::tile_t>& tiles, const std::array<std::uint32_t, WIDTH * HEIGHT>& texture_ids, const std::array<glm::vec4, WIDTH * HEIGHT>& colors)
{
    Grid<WIDTH, HEIGHT> grid(tiles);
    drawGrid(grid, texture_ids, colors);
}
