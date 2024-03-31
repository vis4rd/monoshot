#include "../../include/ecs/systems.hpp"

#include <resource/ResourceManager.hpp>

#include "../../include/ecs/actions.hpp"

namespace ecs::system
{

void moveBullets(entt::registry& registry)
{
    namespace ec = ecs::component;
    const float& delta_time = static_cast<float>(ResourceManager::timer->deltaTime());
    auto view = registry.view<const ec::Rotation, const ec::MaxVelocity, const ec::Acceleration>(
        entt::exclude<ec::Destroyed>);
    for(auto&& [bullet, rot, mvel, acc] : view.each())
    {
        const auto& vel = registry.patch<ec::Velocity>(
            bullet,
            [&acc = acc.data, &mvel = mvel.data, delta_time](float& vel) {
                vel = glm::max(glm::min(vel + acc * delta_time, mvel), 0.f);
            });

        const auto rad = glm::radians(rot.data);
        glm::vec2 rot_vec = {glm::cos(rad), glm::sin(rad)};
        registry.patch<ec::Position>(bullet, [&vel, rot_vec, delta_time](glm::vec2& pos) {
            pos += (vel.data * rot_vec * delta_time);
        });
    }
}

void checkAliveBullets(entt::registry& registry)
{
    const double timestamp = ResourceManager::timer->getTotalTime();
    auto view =
        registry.view<const ecs::component::Lifetime>(entt::exclude<ecs::component::Destroyed>);
    for(auto&& [bullet, life] : view.each())
    {
        if((life.creation + life.timeTillDeath) <= timestamp)
        {
            registry.emplace<ecs::component::Destroyed>(bullet);
            continue;
        }
    }
}

void destroyEntities(entt::registry& registry)
{
    namespace ec = ecs::component;
    const auto view = registry.view<ec::Destroyed>();
    registry.destroy(view.begin(), view.end());
}

void moveHero(entt::registry& registry, Hero& hero, glm::vec2& hero_move_direction)
{
    constexpr auto isOppositeDirectionVector = [](const glm::vec2& one,
                                                  const glm::vec2& two) -> bool {
        return (one.x > 0.f && two.x < 0.f) || (one.x < 0.f && two.x > 0.f)
               || (one.y > 0.f && two.y < 0.f) || (one.y < 0.f && two.y > 0.f);
    };

    const float& delta_time = static_cast<float>(ResourceManager::timer->deltaTime());

    // sort map tiles by distance to the hero
    registry.sort<ecs::component::Position>([&hero](const auto& lhs, const auto& rhs) {
        return glm::abs(glm::length(lhs - hero.position))
               < glm::abs(glm::length(rhs - hero.position));
    });

    // resolve movement variables
    const bool does_hero_move = ((hero_move_direction.x != 0.f) | (hero_move_direction.y != 0.f));
    if(does_hero_move)  // TODO(vis4rd): this branch can be most probably removed
    {
        hero_move_direction = glm::normalize(hero_move_direction);
        hero.velocity =
            (hero.velocity + hero.m_acceleration * delta_time)
            * static_cast<float>(
                not isOppositeDirectionVector(hero_move_direction, hero.walkingDirection));
        hero.velocity = std::min(hero.velocity, hero.m_maxVelocity);
        hero.walkingDirection = hero_move_direction;
    }
    else  // hero does not additionally move in this frame/tick
    {
        hero.velocity -= hero.m_acceleration * delta_time;
        hero.velocity = std::max(hero.velocity, 0.f);
        hero.walkingDirection *= (hero.velocity != 0.f);
    }

    // prepare next step shifts
    auto shift_x = glm::vec2(hero.walkingDirection.x, 0.f) * hero.velocity * delta_time;
    auto shift_y = glm::vec2(0.f, hero.walkingDirection.y) * hero.velocity * delta_time;

    // there should not be much penalty for doing both axes separatly because 99.9% of situations
    // there is no collision on first iteration in either case
    hero.position += shift_x;
    hero.position += shift_y;
}

void updateAis(
    entt::registry& enemy_registry,
    const glm::vec2& hero_pos,
    entt::registry& bullet_registry)
{
    namespace ec = ecs::component;
    constexpr auto distance = [](const glm::vec2& pos1, const glm::vec2& pos2) -> float {
        return glm::length(pos1 - pos2);
    };

    auto view =
        enemy_registry
            .view<ec::Position, ec::Rotation, ec::AiState, ec::AiAwareRange, ec::AiWeapon>();
    for(auto&& [enemy, pos, rot, state, range, weapon] : view.each())
    {
        const auto dist = distance(pos, hero_pos);
        // state update
        switch(state)
        {
            case ec::AiState::IDLE:
            {
                if(dist < range)
                {
                    state.data = ec::AiState::AWARE;
                }
                break;
            }
            case ec::AiState::AWARE:
            {
                if(dist > range)
                {
                    state.data = ec::AiState::IDLE;
                    break;
                }
                const auto diff = hero_pos - pos;
                rot.data = glm::degrees(std::atan2(diff.y, diff.x));

                // spawn some bullets
                if(const bool is_used = weapon.useDelayed(); is_used)
                {
                    if(weapon.getAmmoCurrent() > 0)
                    {
                        const auto bullet_pos = pos + glm::normalize(diff) * 1.f;
                        ecs::action::spawnBullet(
                            bullet_registry,
                            bullet_pos,
                            rot,
                            weapon.getBulletVelocity());
                    }
                }
                break;
            }
            default: break;
        }
    }
}

}  // namespace ecs::system
