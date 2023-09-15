#pragma once

#include <glm/glm.hpp>

struct QuadVertex
{
    glm::vec3 position;
    glm::vec4 color;
    glm::vec2 texCoords;
    float texIndex;
};
