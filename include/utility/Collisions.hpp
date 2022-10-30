#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace OBB  // Oriented Bounding Box
{

struct Polygon
{
    Polygon(const glm::vec2& center, const glm::vec2& size, const float& rotation);
    std::vector<glm::vec2> points;
    glm::vec2 position;
};

glm::vec2 findCollisionSat(glm::vec2& pos1, const glm::vec2& size1, const float& rotation1, glm::vec2& pos2, const glm::vec2& size2, const float& rotation2);
bool findCollisionDiag(glm::vec2& pos1, const glm::vec2& size1, const float& rotation1, glm::vec2& pos2, const glm::vec2& size2, const float& rotation2);

}  // namespace OBB

namespace AABB  // Axis-Aligned Bounding Box
{

bool isCollidingX(const glm::vec2& pos1, const glm::vec2& size1, const glm::vec2& pos2, const glm::vec2& size2);
bool isCollidingY(const glm::vec2& pos1, const glm::vec2& size1, const glm::vec2& pos2, const glm::vec2& size2);
bool isColliding(const glm::vec2& pos1, const glm::vec2& size1, const glm::vec2& pos2, const glm::vec2& size2);
glm::vec2 findCollision(const glm::vec2& pos1, const glm::vec2& size1, const glm::vec2& pos2, const glm::vec2& size2);

}  // namespace AABB
