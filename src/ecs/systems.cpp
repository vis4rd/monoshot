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
    const auto is_colliding_with_anything = [&hero_id, &registry](const glm::vec2& next_pos) -> bool
    {
        constexpr glm::vec2 hero_size{0.6f, 0.6f};
        const auto view = registry.view<const ecs::component::position>();
        // const float& hero_rot = registry.get<const ecs::component::rotation>(hero_id);
        for(std::int32_t iter = 0; const auto& element : view)
        {
            if(iter >= 4)
            {
                break;
            }
            if(element == hero_id)
            {
                continue;
            }
            const glm::vec2& tile_pos = view.get<const ecs::component::position>(element);
            const float& tile_rot = registry.get<const ecs::component::rotation>(element);
            const glm::vec2& tile_size = registry.get<const ecs::component::size>(element);

            // TODO: unfortunately hero rotation sometimes causes the character to get stuck, pls fix
            if(OBB::findCollision(next_pos, hero_size, /*hero_rot*/ 0.f, tile_pos, tile_size, tile_rot))
            {
                return true;
            }

            iter++;
        }
        return false;
    };

    const auto resolve_collision = [&is_colliding_with_anything](glm::vec2& current_pos, glm::vec2 step) -> void
    {
        // brute-force checking whther there is a collision in the next step
        std::size_t tries = 1;
        for(; is_colliding_with_anything(current_pos + step) && tries < 5; tries++)
        {
            step /= 2.f;
        }
        if(tries < 5)
        {
            current_pos += step;
        }
    };

    const float& delta_time = ResourceManager::timer->deltaTime();
    glm::vec2& pos = registry.get<ecs::component::position>(hero_id);
    float& vel = registry.get<ecs::component::velocity>(hero_id);
    const float& mvel = registry.get<const ecs::component::max_velocity>(hero_id);
    float& acc = registry.get<ecs::component::acceleration>(hero_id);
    glm::vec2& dir = registry.get<ecs::component::direction>(hero_id);

    // sort map tiles by distance to the hero
    registry.sort<ecs::component::position>(
        [&pos](const auto& lhs, const auto& rhs)
        {
            return glm::abs(glm::length(lhs - pos)) < glm::abs(glm::length(rhs - pos));
        });

    // resolve movement variables
    const bool does_hero_move = (hero_move_direction.x != 0.f) || (hero_move_direction.y != 0.f);
    if(does_hero_move)
    {
        dir = hero_move_direction;
        vel += acc * delta_time;
        if(vel > mvel)
        {
            vel = mvel;
        }
    }
    else  // hero does not additionally move in this frame/tick
    {
        vel -= acc * delta_time;
        if(vel < 0.f)
        {
            vel = 0.f;
            dir = {0.f, 0.f};
        }
    }

    // prepare next step shifts
    auto shift_x = glm::vec2(dir.x, 0.f) * vel * delta_time;
    auto shift_y = glm::vec2(0.f, dir.y) * vel * delta_time;

    // there should not be much penalty for doing both axes separatly because 99.9% of situations there is no collision on first iteration in either case
    resolve_collision(pos, shift_x);
    resolve_collision(pos, shift_y);
}

}  // namespace ecs::system
