#include "../../include/utility/Collisions.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <spdlog/spdlog.h>

namespace OBB
{

// Separate Axis Theorem implementation
// See: https://en.wikipedia.org/wiki/Hyperplane_separation_theorem

Polygon::Polygon(const glm::vec2& center, const glm::vec2& size, const float& rotation)
    : points(4),
      position(center)
{
    //? possibly this matrix operation can be simplified if this path becomes too hot
    glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3(center, 0.f));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.f, 0.f, 1.f));
    model = glm::scale(model, glm::vec3(size, 1.f));

    this->points[0] = glm::vec2(model * glm::vec4{-0.5f, -0.5f, 0.f, 1.f});
    this->points[1] = glm::vec2(model * glm::vec4{0.5f, -0.5f, 0.f, 1.f});
    this->points[2] = glm::vec2(model * glm::vec4{0.5f, 0.5f, 0.f, 1.f});
    this->points[3] = glm::vec2(model * glm::vec4{-0.5f, 0.5f, 0.f, 1.f});
}

glm::vec2 findCollisionSat(glm::vec2& pos1, const glm::vec2& size1, const float& rotation1, glm::vec2& pos2, const glm::vec2& size2, const float& rotation2)
{
    constexpr float inf = 999999999.f;
    Polygon poly1(pos1, size1, 0.f);
    Polygon poly2(pos2, size2, rotation2);

    float overlap = inf;
    auto resolve = [&overlap](Polygon& poly1, Polygon& poly2) mutable -> bool
    {
        for(int a = 0; a < poly1.points.size(); a++)
        {
            int b = (a + 1) % poly1.points.size();
            glm::vec2 axis_proj = glm::normalize(glm::vec2{-(poly1.points[b].y - poly1.points[a].y), poly1.points[b].x - poly1.points[a].x});

            float min_r1 = inf;
            float max_r1 = -inf;
            for(int p = 0; p < poly1.points.size(); p++)
            {
                float q = (poly1.points[p].x * axis_proj.x) + (poly1.points[p].y * axis_proj.y);
                min_r1 = glm::min(min_r1, q);
                max_r1 = glm::max(max_r1, q);
            }

            float min_r2 = inf;
            float max_r2 = -inf;
            for(int p = 0; p < poly2.points.size(); p++)
            {
                float q = (poly2.points[p].x * axis_proj.x) + (poly2.points[p].y * axis_proj.y);
                min_r2 = glm::min(min_r2, q);
                max_r2 = glm::max(max_r2, q);
            }

            overlap = glm::min(glm::min(max_r1, max_r2) - glm::max(min_r1, min_r2), overlap);

            if(!(max_r2 >= min_r1 && max_r1 >= min_r2))
            {
                return false;
            }
        }
        return true;
    };
    if(!resolve(poly1, poly2) || !resolve(poly2, poly1))
    {
        return {0.f, 0.f};
    }

    glm::vec2 d = glm::normalize(pos2 - pos1);
    return -overlap * d;
}

/**
 * @brief Find intersection between two line segments.
 *
 * @warning This function assumes that the intersection exists. If it doesn't, the behavior is undefined.
 * @return glm::vec2
 */
// glm::vec2 findSegmentIntersection(const glm::vec2& one_s, const glm::vec2& one_e, const glm::vec2& two_s, const glm::vec2& two_e)
// {
//     constexpr auto cross = [](const glm::vec2& one, const glm::vec2& two) -> float
//     {
//         return one.x * two.y - one.y * two.x;
//     };
//     const auto one = one_e - one_s;
//     const auto two = two_e - two_s;
//     const auto one_x_two = cross(one, two);
//     const auto tsos_x_one = cross(two_s - one_s, one);
//     const auto tsos_x_two = cross(two_s - one_s, two);

// const auto t = tsos_x_two / one_x_two;
// const auto u = tsos_x_one / one_x_two;
// return one_s + t * one;}


// Polygon "Diagonal" Collision Algorithm (not an official name)
// source: https://youtu.be/7Ik2vowGcU0

bool findCollision(const glm::vec2& pos1, const glm::vec2& size1, const float& rotation1, const glm::vec2& pos2, const glm::vec2& size2, const float& rotation2)
{
    constexpr auto doSegmentsIntersect = [](const glm::vec2& line1_s, const glm::vec2& line1_e, const glm::vec2& line2_s, const glm::vec2& line2_e) -> bool
    {
        // TODO: seems like this code does not cover some corner cases, possibly replace it with some more refined one
        return (((line2_s.x - line1_s.x) * (line1_e.y - line1_s.y) - (line2_s.y - line1_s.y) * (line1_e.x - line1_s.x)) * ((line2_e.x - line1_s.x) * (line1_e.y - line1_s.y) - (line2_e.y - line1_s.y) * (line1_e.x - line1_s.x)) < 0)
               && (((line1_s.x - line2_s.x) * (line2_e.y - line2_s.y) - (line1_s.y - line2_s.y) * (line2_e.x - line2_s.x)) * ((line1_e.x - line2_s.x) * (line2_e.y - line2_s.y) - (line1_e.y - line2_s.y) * (line2_e.x - line2_s.x)) < 0);
    };

    // TODO: remove Polygons and simplify this function if possible
    const Polygon poly1(pos1, size1, rotation1);
    const Polygon poly2(pos2, size2, rotation2);

    for(int poly1_point_index = 0; poly1_point_index < poly1.points.size(); poly1_point_index++)
    {
        glm::vec2 poly1_center = poly1.position;
        glm::vec2 poly1_point = poly1.points[poly1_point_index];

        for(int poly2_point_index = 0; poly2_point_index < poly2.points.size(); poly2_point_index++)
        {
            glm::vec2 poly2_point1 = poly2.points[poly2_point_index];
            glm::vec2 poly2_point2 = poly2.points[(poly2_point_index + 1) % poly2.points.size()];

            // check if poly1's center -> vertex line segment intersects with poly2's edge
            if(doSegmentsIntersect(poly1_center, poly1_point, poly2_point1, poly2_point2))
            {
                return true;
            }
        }
    }
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
