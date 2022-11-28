#include "../../include/ecs/systems.hpp"

#include "../../include/utility/Collisions.hpp"
#include "../../include/utility/ResourceManager.hpp"

namespace ecs::system
{

namespace impl
{

bool is_colliding_with_anything(entt::registry& registry, const glm::vec2& entity_size, const glm::vec2& next_pos)
{
    const auto view = registry.view<const ecs::component::position>();
    // const float& hero_rot = registry.get<const ecs::component::rotation>(entity);
    for(std::int32_t iter = 0; const auto& element : view)
    {
        if(iter >= 4)
        {
            break;
        }
        const glm::vec2& tile_pos = view.get<const ecs::component::position>(element);
        const float& tile_rot = registry.get<const ecs::component::rotation>(element);
        const glm::vec2& tile_size = registry.get<const ecs::component::size>(element);

        // TODO: unfortunately entity rotation sometimes causes the character to get stuck, pls fix
        if(OBB::findCollision(next_pos, entity_size, /*entity_rot*/ 0.f, tile_pos, tile_size, tile_rot))
        {
            return true;
        }

        iter++;
    }
    return false;
}

}  // namespace impl

void move_bullets(entt::registry& registry)
{
    using namespace ecs::component;
    const float& delta_time = ResourceManager::timer->deltaTime();
    auto view = registry.view<const rotation, const max_velocity, const acceleration>();
    for(auto&& [bullet, rot, mvel, acc] : view.each())
    {
        const auto& vel = registry.patch<velocity>(bullet,
            [&acc = acc.data, &mvel = mvel.data, delta_time](float& vel)
            {
                vel = glm::max(glm::min(vel + acc * delta_time, mvel), 0.f);
            });

        const auto rad = glm::radians(rot.data);
        glm::vec2 rot_vec = {glm::cos(rad), glm::sin(rad)};
        registry.patch<position>(bullet,
            [&vel, rot_vec, delta_time](glm::vec2& pos)
            {
                pos += (vel.data * rot_vec * delta_time);
            });
    }
}

void destroy_bullets(entt::registry& registry)
{
    const double timestamp = ResourceManager::timer->getTotalTime();
    auto view = registry.view<const ecs::component::lifetime>();
    for(auto&& [bullet, life] : view.each())
    {
        if((life.creation + life.timeTillDeath) <= timestamp)
        {
            registry.destroy(bullet);
            continue;
        }
    }
}

void move_hero_with_collisions(entt::registry& registry, Hero& hero, glm::vec2& hero_move_direction)
{
    const auto resolve_collision = [&registry](glm::vec2& current_pos, const glm::vec2& size, glm::vec2 step) -> void
    {
        // brute-force checking whether there is a collision in the next step
        constexpr std::size_t max_tries = 3;
        std::size_t tries = 1;
        for(; impl::is_colliding_with_anything(registry, size, current_pos + step) && tries < max_tries; tries++)
        {
            step /= 2.f;
        }
        if(tries < max_tries)
        {
            current_pos += step;
        }
    };

    const float& delta_time = ResourceManager::timer->deltaTime();

    // sort map tiles by distance to the hero
    registry.sort<ecs::component::position>(
        [&hero](const auto& lhs, const auto& rhs)
        {
            return glm::abs(glm::length(lhs - hero.position)) < glm::abs(glm::length(rhs - hero.position));
        });

    // resolve movement variables
    const bool does_hero_move = ((hero_move_direction.x != 0.f) | (hero_move_direction.y != 0.f));
    if(does_hero_move)  // TODO: this branch can be most probably removed
    {
        hero.walkingDirection = hero_move_direction;
        hero.velocity += hero.acceleration * delta_time;
        hero.velocity = std::min(hero.velocity, hero.maxVelocity);
        // spdlog::trace("Hero moves");
    }
    else  // hero does not additionally move in this frame/tick
    {
        hero.velocity -= hero.acceleration * delta_time;
        hero.velocity = std::max(hero.velocity, 0.f);
        hero.walkingDirection *= bool(hero.velocity != 0.f);
        // spdlog::trace("Hero does not move: vel = {}, calced = {:d}", hero.velocity, ((hero.walkingDirection.x != 0.f) | (hero.walkingDirection.y != 0.f)));
    }

    // prepare next step shifts
    auto shift_x = glm::vec2(hero.walkingDirection.x, 0.f) * hero.velocity * delta_time;
    auto shift_y = glm::vec2(0.f, hero.walkingDirection.y) * hero.velocity * delta_time;

    // there should not be much penalty for doing both axes separatly because 99.9% of situations there is no collision on first iteration in either case
    resolve_collision(hero.position, hero.size, shift_x);
    resolve_collision(hero.position, hero.size, shift_y);
}

}  // namespace ecs::system
