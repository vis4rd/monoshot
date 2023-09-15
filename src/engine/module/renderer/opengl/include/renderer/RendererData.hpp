#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "../gl/VertexArray.hpp"
#include "../texture/Texture.hpp"
#include "LineVertex.hpp"
#include "QuadVertex.hpp"

struct RendererData
{
    static constinit const std::size_t maxLineCount = 10000;
    static constinit const std::size_t maxQuadCount = 10000;
    static constinit const std::size_t maxVertexCount = maxQuadCount * 4;
    static constinit const std::size_t maxIndexCount = maxQuadCount * 6;
    static constinit const std::size_t maxTextures = 32;

    std::shared_ptr<mono::VertexArray> quadVao = nullptr;
    std::vector<QuadVertex> quadBuffer = std::vector<QuadVertex>(maxVertexCount);
    std::vector<QuadVertex>::iterator quadBufferIter{};

    std::shared_ptr<mono::VertexArray> lineVao = nullptr;
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
};
