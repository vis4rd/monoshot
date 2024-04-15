#pragma once

#include <array>

#include <glm/fwd.hpp>

#include "mono/util/PackedVariable.hpp"

namespace mono::gl
{

namespace detail
{

constexpr int tex_index = 5;
constexpr int rotation = 9;
constexpr int unused = (sizeof(glm::uint32) * 8) - rotation - tex_index;
using RtiPacked = mono::util::PackedVariable<glm::uint32, unused, tex_index, rotation>;
// RtiPacked memory layout (left to right):
// unused:          = 18 bits
// texIndex: 0-31   = 5 bits
// rotation: 0-359  = 9 bits
// 000000000000000000tttttrrrrrrrrr

}  // namespace detail

struct QuadInstanceData
{
    glm::uint32 color;
    glm::vec2 position;
    glm::vec2 scale;
    detail::RtiPacked rotation_texIndex;
};

static constexpr std::array<glm::vec2, 8> quadConstantVertexData = {
    glm::vec2{-0.5f, -0.5f}, // pos left-top
    glm::vec2{0.0f,  0.0f }, // tex left-top
    glm::vec2{0.5f,  -0.5f}, // pos right-top
    glm::vec2{1.0f,  0.0f }, // tex right-top
    glm::vec2{0.5f,  0.5f }, // pos right-bottom
    glm::vec2{1.0f,  1.0f }, // tex right-bottom
    glm::vec2{-0.5f, 0.5f }, // pos left-bottom
    glm::vec2{0.0f,  1.0f }, // tex left-bottom
};

}  // namespace mono::gl
