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

bool findCollisionDiag(glm::vec2& pos1, const glm::vec2& size1, const float& rotation1, glm::vec2& pos2, const glm::vec2& size2, const float& rotation2)
{
    Polygon r1(pos1, size1, 0.f);
    Polygon r2(pos2, size2, rotation2);

    auto* poly1 = &r1;
    auto* poly2 = &r2;

    for(int shape = 0; shape < 2; shape++)
    {
        if(shape == 1)
        {
            poly1 = &r2;
            poly2 = &r1;
        }

        // Check diagonals of this polygon...
        for(int p = 0; p < poly1->points.size(); p++)
        {
            glm::vec2 line_r1s = poly1->position;
            glm::vec2 line_r1e = poly1->points[p];

            glm::vec2 displacement(0.f, 0.f);

            // ...against edges of this polygon
            for(int q = 0; q < poly2->points.size(); q++)
            {
                glm::vec2 line_r2s = poly2->points[q];
                glm::vec2 line_r2e = poly2->points[(q + 1) % poly2->points.size()];

                // Standard "off the shelf" line segment intersection
                float h = (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r1e.y) - (line_r1s.x - line_r1e.x) * (line_r2e.y - line_r2s.y);
                float t1 = ((line_r2s.y - line_r2e.y) * (line_r1s.x - line_r2s.x) + (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r2s.y)) / h;
                float t2 = ((line_r1s.y - line_r1e.y) * (line_r1s.x - line_r2s.x) + (line_r1e.x - line_r1s.x) * (line_r1s.y - line_r2s.y)) / h;

                if(t1 >= 0.0f && t1 < 1.0f && t2 >= 0.0f && t2 < 1.0f)
                {
                    displacement += (1.0f - t1) * (line_r1e - line_r1s);
                }
            }
            pos1 += displacement * (shape == 0 ? -1.f : 1.f);
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
