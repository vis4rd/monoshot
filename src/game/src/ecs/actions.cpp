#include "../../include/ecs/actions.hpp"

#include "../../include/ecs/components.hpp"

namespace ecs::action
{


void spawn_enemy(
    entt::registry& registry,
    const glm::vec2& pos,
    const glm::vec2& size,
    const float& rot,
    const std::int32_t& health)
{
    namespace ec = ecs::component;
    auto enemy = registry.create();
    registry.emplace<ec::position>(enemy, pos);
    registry.emplace<ec::size>(enemy, size);
    registry.emplace<ec::rotation>(enemy, rot);
    registry.emplace<ec::health>(enemy, health);
    registry.emplace<ec::ai_state>(enemy);
    registry.emplace<ec::ai_aware_range>(enemy, 5.f);
    registry.emplace<ec::ai_weapon>(enemy, 20, 31, 99999, 35.f, 0.3);
}

void spawn_bullet(
    entt::registry& registry,
    const glm::vec2& pos,
    const float& rot,
    const float& vel)
{
    namespace ec = ecs::component;
    auto bullet = registry.create();
    registry.emplace<ec::lifetime>(bullet, ResourceManager::timer->getTotalTime(), 2.0);
    registry.emplace<ec::position>(bullet, pos);
    registry.emplace<ec::size>(bullet, glm::vec2{0.1f, 0.1f});
    registry.emplace<ec::velocity>(bullet, vel);
    registry.emplace<ec::max_velocity>(bullet, vel);
    registry.emplace<ec::acceleration>(bullet, -1.f);
    registry.emplace<ec::rotation>(bullet, rot);
};

}  // namespace ecs::action
