#include "../../include/utility/Collisions.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <spdlog/spdlog.h>

namespace OBB
{

// Separate Axis Theorem implementation
// See: https://en.wikipedia.org/wiki/Hyperplane_separation_theorem

namespace impl
{

bool getSeparatingLine(const glm::vec2& distance, const glm::vec2& axis, const BB& box1, const BB& box2)
{
    // Calculate sum of lengths of axis projections and axist projection of distance between cubes
    // If said distance is greater than the sum, there is a collision
    // clang-format off
    return (std::fabs(glm::dot(distance, axis)) > (
        std::fabs(glm::dot(box1.axisX*box1.halfSize.x, axis)) +
        std::fabs(glm::dot(box1.axisY*box1.halfSize.y, axis)) +
        std::fabs(glm::dot(box2.axisX*box2.halfSize.x, axis)) +
        std::fabs(glm::dot(box2.axisY*box2.halfSize.y, axis))
    ));
    // clang-format on
}

}  // namespace impl

BB createBoundingBox(const glm::vec2& center, const glm::vec2& size, const float& rotation)
{
    const float angle = glm::radians(rotation);
    const glm::mat2 rot = {glm::cos(angle), glm::sin(angle), -glm::sin(angle), glm::cos(angle)};

    BB result;
    result.halfSize = size / 2.f;
    result.axisX = rot * glm::vec2{1.f, 0.f};
    result.axisY = rot * glm::vec2{0.f, 1.f};
    result.pos = center;

    return result;
}

bool getCollision(const BB& box1, const BB& box2)
{
    static glm::vec2 distance;  // distance between centers of two cubes
    distance = box2.pos - box1.pos;

    // clang-format off
    return not (
        impl::getSeparatingLine(distance, box1.axisX, box1, box2) ||
        impl::getSeparatingLine(distance, box1.axisY, box1, box2) ||
        impl::getSeparatingLine(distance, box1.axisX*box2.axisX, box1, box2) ||
        impl::getSeparatingLine(distance, box1.axisX*box2.axisY, box1, box2) ||
        impl::getSeparatingLine(distance, box1.axisY*box2.axisX, box1, box2) ||
        impl::getSeparatingLine(distance, box1.axisY*box2.axisY, box1, box2)
    );
    // clang-format on
    return false;
}

}  // namespace OBB

namespace AABB
{

bool isCollidingX(const glm::vec2& pos1, const glm::vec2& size1, const glm::vec2& pos2, const glm::vec2& size2)
{
    const auto left1 = pos1.x - (size1.x / 2.f);
    const auto right1 = pos1.x + (size1.x / 2.f);
    const auto left2 = pos2.x - (size2.x / 2.f);
    const auto right2 = pos2.x + (size2.x / 2.f);
    return (left1 < right2) && (right1 > left2);
}

bool isCollidingY(const glm::vec2& pos1, const glm::vec2& size1, const glm::vec2& pos2, const glm::vec2& size2)
{
    const auto bottom1 = pos1.y - (size1.y / 2.f);
    const auto top1 = pos1.y + (size1.y / 2.f);
    const auto bottom2 = pos2.y - (size2.y / 2.f);
    const auto top2 = pos2.y + (size2.y / 2.f);
    return (bottom1 < top2) && (top1 > bottom2);
}

bool isColliding(const glm::vec2& pos1, const glm::vec2& size1, const glm::vec2& pos2, const glm::vec2& size2)
{
    return isCollidingX(pos1, size1, pos2, size2) && isCollidingY(pos1, size1, pos2, size2);
}

glm::vec2 findCollision(const glm::vec2& pos1, const glm::vec2& size1, const glm::vec2& pos2, const glm::vec2& size2)
{
    // find intersection area
    const float inter_left = glm::max(pos1.x - size1.x / 2.f, pos2.x - size2.x / 2.f);
    const float inter_bottom = glm::max(pos1.y - size1.y / 2.f, pos2.y - size2.y / 2.f);
    const float inter_right = glm::min(pos1.x + size1.x / 2.f, pos2.x + size2.x / 2.f);
    const float inter_top = glm::min(pos1.y + size1.y / 2.f, pos2.y + size2.y / 2.f);

    return {inter_right - inter_left, inter_top - inter_bottom};
}

}  // namespace AABB
