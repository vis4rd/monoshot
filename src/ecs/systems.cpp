#include "../../include/ecs/systems.hpp"

#include "../../include/utility/Collisions.hpp"
#include "../../include/utility/ResourceManager.hpp"

namespace ecs::system
{

void move_based_on_rotation(entt::registry& registry, const float& delta_time) { }

void move(entt::registry& registry, const float& delta_time)
{
    using namespace ecs::component;
    auto view = registry.view<position, rotation, velocity, max_velocity, acceleration>();
    for(const auto& entity : view)
    {
        glm::vec2& pos = view.get<position>(entity);
        float& rot = view.get<rotation>(entity);
        float& vel = view.get<velocity>(entity);
        float& mvel = view.get<max_velocity>(entity);
        float& acc = view.get<acceleration>(entity);

        vel = glm::min(vel + acc * delta_time, mvel * delta_time);
        auto rad = glm::radians(rot);
        glm::vec2 rot_vec = {glm::cos(rad), glm::sin(rad)};
        pos += (vel * rot_vec * delta_time);
    }
}

void collide_with_hero(entt::registry& registry, const entt::entity& hero_id, glm::vec2& hero_move_direction)
{
    constexpr glm::vec2 hero_size{0.6f, 0.6f};
    constexpr auto signum = []<typename T>(T val) -> float
    {
        return (T(0) < val) - (val < T(0));
    };

    auto& delta_time = ResourceManager::timer->deltaTime();
    glm::vec2& pos = registry.get<ecs::component::position>(hero_id);
    float& vel = registry.get<ecs::component::velocity>(hero_id);
    float& mvel = registry.get<ecs::component::max_velocity>(hero_id);
    float& acc = registry.get<ecs::component::acceleration>(hero_id);
    glm::vec2& dir = registry.get<ecs::component::direction>(hero_id);
    float& rot = registry.get<ecs::component::rotation>(hero_id);

    // sort map tiles by distance to the hero
    registry.sort<ecs::component::position>(
        [&pos](const auto& lhs, const auto& rhs)
        {
            return glm::abs(glm::length(lhs - pos)) < glm::abs(glm::length(rhs - pos));
        });

    if(hero_move_direction != glm::vec2(0.f, 0.f))
    {
        // calculate velocity and next position
        auto length = glm::length(hero_move_direction);
        if(length > 0.f)
        {
            dir = hero_move_direction;

            vel += acc * delta_time;
            if(vel > mvel)
            {
                vel = mvel;
            }
        }
        auto next_pos = pos + vel * dir * static_cast<float>(delta_time);

        // forbid moving into solid tiles
        const auto view = registry.view<ecs::component::position>();

        for(std::int32_t iter = 0; const auto& element : view)
        {
            if(element == hero_id)
            {
                continue;
            }
            if(iter >= 4)
            {
                break;
            }
            const auto& tile_pos = view.get<ecs::component::position>(element);
            const auto& tile_rot = 0.f;
            const glm::vec2& tile_size = registry.get<ecs::component::size>(element);

            const auto intersection = AABB::findCollision(next_pos, hero_size, tile_pos, tile_size);
            if(intersection.x > 0.f && intersection.y > 0.f)
            {
                glm::vec2 center_diff = glm::abs(tile_pos - pos);
                glm::vec2 sum_of_sizes = hero_size + tile_size;
                glm::vec2 ncd = center_diff / (sum_of_sizes / 2.f);  // normalized center difference
                // glm::vec2 nss = glm::normalize(sum_of_sizes);  // normalized sum of sizes

                if(center_diff.x * std::abs(ncd.x) > center_diff.y * std::abs(ncd.y))  // collision is horizontal
                //? consider multiplying both by nss.x and nss.y
                {
                    next_pos.x = next_pos.x - intersection.x * signum(dir.x);
                    // next_pos.y = pos.y;  // TODO: cut by the factor of how much shorter is the x movement
                    //? (this is probably the cause of this stuttering)
                    // currently it is not really necessary as incorrect offset values are truly minimal
                    dir.x = 0.f;
                }
                else  // collision is vertical
                {
                    next_pos.y = next_pos.y - intersection.y * signum(dir.y);
                    // next_pos.x = pos.x;  // TODO: cut by the factor of how much shorter is the y movement
                    //? (this is probably the cause of this stuttering)
                    // currently it is not really necessary as incorrect offset values are truly minimal
                    dir.y = 0.f;
                }
            }

            iter++;
        }

        pos = next_pos;
    }
    else
    {
        vel -= acc * delta_time;
        if(vel < 0.f)
        {
            vel = 0.f;
        }
    }
}  // collide_with_hero()

}  // namespace ecs::system
