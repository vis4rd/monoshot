#include "../../include/ecs/systems.hpp"

#include <resource/ResourceManager.hpp>

#include "../../include/ecs/actions.hpp"
#include "../../include/utility/Collisions.hpp"

namespace ecs::system
{

namespace impl
{

bool isCollidingWithAnything(
    entt::registry& registry,
    const glm::vec2& entity_size,
    const glm::vec2& next_pos)
{
    using namespace ecs::component;
    auto view = registry.view<const Position, const Rotation, const Size>();
    view.use<const Position>();
    // const float& hero_rot = registry.get<const rotation>(entity);
    for(std::int32_t iter = 0; auto&& [element, el_pos, el_rot, el_size] : view.each())
    {
        if(iter >= 4)
        {
            break;
        }

        // TODO: unfortunately entity rotation sometimes causes the character to get stuck, pls fix
        if(OBB::findCollision(next_pos, entity_size, /*entity_rot*/ 0.f, el_pos, el_size, el_rot))
        {
            return true;
        }

        iter++;
    }
    return false;
}

}  // namespace impl

void moveBullets(entt::registry& registry)
{
    using namespace ecs::component;
    const float& delta_time = ResourceManager::timer->deltaTime();
    auto view = registry.view<const Rotation, const MaxVelocity, const Acceleration>(
        entt::exclude<ecs::component::Destroyed>);
    for(auto&& [bullet, rot, mvel, acc] : view.each())
    {
        const auto& vel = registry.patch<Velocity>(
            bullet,
            [&acc = acc.data, &mvel = mvel.data, delta_time](float& vel) {
                vel = glm::max(glm::min(vel + acc * delta_time, mvel), 0.f);
            });

        const auto rad = glm::radians(rot.data);
        glm::vec2 rot_vec = {glm::cos(rad), glm::sin(rad)};
        registry.patch<Position>(bullet, [&vel, rot_vec, delta_time](glm::vec2& pos) {
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

void collideBullets(
    entt::registry& bullet_registry,
    entt::registry& map_registry,
    entt::registry& enemy_registry)
{
    namespace ec = ecs::component;

    auto bullet_view = bullet_registry.view<ec::Position, ec::Size>(entt::exclude<ec::Destroyed>);
    auto map_view = map_registry.view<ec::Position, ec::Size, ec::Rotation>();
    auto enemy_view = enemy_registry.view<ec::Position, ec::Size, ec::Rotation, ec::Health>();
    bullet_view.use<ec::Position>();
    map_view.use<ec::Position>();
    enemy_view.use<ec::Position>();

    // go through every bullet
    for(auto&& [bullet_entity, b_pos, b_size] : bullet_view.each())
    {
        if(not bullet_registry.valid(bullet_entity))
        {
            continue;
        }

        const auto element_sorter = [&b_pos = b_pos](const auto& lhs, const auto& rhs) {
            return glm::abs(glm::length(lhs - b_pos)) < glm::abs(glm::length(rhs - b_pos));
        };

        // ENEMIES RESOLUTION
        // sort enemies in similar order to bullets, hoping that most swaps will be skipped (when
        // bullets have similar positions)
        enemy_registry.sort<ec::Position>(element_sorter);

        // go through closest enemies
        const auto& en_entity = enemy_view.front();
        if(en_entity != entt::null)
        {
            if(OBB::findCollision(
                   b_pos,
                   b_size,
                   0.f,
                   enemy_view.get<ec::Position>(en_entity),
                   enemy_view.get<ec::Size>(en_entity),
                   enemy_view.get<ec::Rotation>(en_entity)))
            {
                std::int32_t& enemy_health = enemy_view.get<ec::Health>(en_entity);
                enemy_health -= 20;
                if(enemy_health <= 0)
                {
                    enemy_registry.emplace_or_replace<ec::Destroyed>(en_entity);
                }
                bullet_registry.emplace_or_replace<ec::Destroyed>(bullet_entity);
            }
        }

        // MAP RESOLUTION
        // sort map elements in similar order to bullets, hoping that most swaps will be skipped
        // (when bullets have similar positions)
        map_registry.sort<ec::Position>(element_sorter);

        // go through closest map elements
        const auto& el_entity = map_view.front();
        if(OBB::findCollision(
               b_pos,
               b_size,
               0.f,
               map_view.get<ec::Position>(el_entity),
               map_view.get<ec::Size>(el_entity),
               map_view.get<ec::Rotation>(el_entity)))
        {
            bullet_registry.emplace_or_replace<ec::Destroyed>(bullet_entity);
        }
    }
}

void destroyEntities(entt::registry& registry)
{
    namespace ec = ecs::component;
    const auto view = registry.view<ec::Destroyed>();
    registry.destroy(view.begin(), view.end());
}

void moveHeroWithCollisions(entt::registry& registry, Hero& hero, glm::vec2& hero_move_direction)
{
    const auto resolve_collision =
        [&registry](glm::vec2& current_pos, const glm::vec2& size, glm::vec2 step) -> void {
        // brute-force checking whether there is a collision in the next step
        constexpr std::size_t maxTries = 3;
        std::size_t tries = 1;
        for(; impl::isCollidingWithAnything(registry, size, current_pos + step) && tries < maxTries;
            tries++)
        {
            step /= 2.f;
        }
        if(tries < maxTries)
        {
            current_pos += step;
        }
    };

    constexpr auto isOppositeDirectionVector = [](const glm::vec2& one,
                                                  const glm::vec2& two) -> bool {
        return (one.x > 0.f && two.x < 0.f) || (one.x < 0.f && two.x > 0.f)
               || (one.y > 0.f && two.y < 0.f) || (one.y < 0.f && two.y > 0.f);
    };

    const float& delta_time = ResourceManager::timer->deltaTime();

    // sort map tiles by distance to the hero
    registry.sort<ecs::component::Position>([&hero](const auto& lhs, const auto& rhs) {
        return glm::abs(glm::length(lhs - hero.position))
               < glm::abs(glm::length(rhs - hero.position));
    });

    // resolve movement variables
    const bool does_hero_move = ((hero_move_direction.x != 0.f) | (hero_move_direction.y != 0.f));
    if(does_hero_move)  // TODO: this branch can be most probably removed
    {
        hero_move_direction = glm::normalize(hero_move_direction);
        hero.velocity =
            (hero.velocity + hero.m_acceleration * delta_time)
            * (not isOppositeDirectionVector(hero_move_direction, hero.walkingDirection));
        hero.velocity = std::min(hero.velocity, hero.m_maxVelocity);
        hero.walkingDirection = hero_move_direction;
        // spdlog::trace("Hero moves");
    }
    else  // hero does not additionally move in this frame/tick
    {
        hero.velocity -= hero.m_acceleration * delta_time;
        hero.velocity = std::max(hero.velocity, 0.f);
        hero.walkingDirection *= (hero.velocity != 0.f);
        // spdlog::trace("Hero does not move: vel = {}, calced = {:d}", hero.velocity,
        // ((hero.walkingDirection.x != 0.f) | (hero.walkingDirection.y != 0.f)));
    }

    // prepare next step shifts
    auto shift_x = glm::vec2(hero.walkingDirection.x, 0.f) * hero.velocity * delta_time;
    auto shift_y = glm::vec2(0.f, hero.walkingDirection.y) * hero.velocity * delta_time;

    // there should not be much penalty for doing both axes separatly because 99.9% of situations
    // there is no collision on first iteration in either case
    resolve_collision(hero.position, hero.m_size, shift_x);
    resolve_collision(hero.position, hero.m_size, shift_y);
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
