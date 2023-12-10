#pragma once

#include <array>

#include <glm/glm.hpp>

namespace mono
{

struct QuadInstanceData
{
    glm::vec4 color;
    float texIndex;
    glm::mat4 model;
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

}  // namespace mono
