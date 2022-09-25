#pragma once

#include <glm/glm.hpp>

namespace OBB  // Oriented Bounding Box
{

struct BB
{
    glm::vec2 pos;
    glm::vec2 axisX;
    glm::vec2 axisY;
    glm::vec2 halfSize;
};

namespace impl
{

bool getSeparatingLine(const glm::vec2& r_pos, const glm::vec2& line, const BB& box1, const BB& box2);

}  // namespace impl

BB createBoundingBox(const glm::vec2& center, const glm::vec2& size, const float& rotation);
bool getCollision(const BB& box1, const BB& box2);

}  // namespace OBB

namespace AABB  // Axis-Aligned Bounding Box
{

bool isCollidingX(const glm::vec2& pos1, const glm::vec2& size1, const glm::vec2& pos2, const glm::vec2& size2);
bool isCollidingY(const glm::vec2& pos1, const glm::vec2& size1, const glm::vec2& pos2, const glm::vec2& size2);
bool isColliding(const glm::vec2& pos1, const glm::vec2& size1, const glm::vec2& pos2, const glm::vec2& size2);
glm::vec2 findCollision(const glm::vec2& pos1, const glm::vec2& size1, const glm::vec2& pos2, const glm::vec2& size2);

}  // namespace AABB
