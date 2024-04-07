#pragma once

#include <array>

#include <glm/fwd.hpp>

namespace mono::gl
{

struct QuadInstanceData
{
    glm::uint32 color;
    glm::vec2 position;
    glm::vec2 scale;
    glm::uint32 rotation_texIndex;
    // rotation_texIndex memory layout (left to right):
    // unused:          = 18 bits
    // texIndex: 0-31   = 5 bits
    // rotation: 0-359  = 9 bits
    // 000000000000000000tttttrrrrrrrrr
    // TODO(vis4rd): create a class to handle these bit shifting shenanigans
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
