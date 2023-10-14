#include "../../include/ecs/actions.hpp"

#include "../../include/ecs/components.hpp"

namespace ecs::action
{


void spawnEnemy(
    entt::registry& registry,
    const glm::vec2& pos,
    const glm::vec2& size,
    const float& rot,
    const std::int32_t& health)
{
    namespace ec = ecs::component;
    auto enemy = registry.create();
    registry.emplace<ec::Position>(enemy, pos);
    registry.emplace<ec::Size>(enemy, size);
    registry.emplace<ec::Rotation>(enemy, rot);
    registry.emplace<ec::Health>(enemy, health);
    registry.emplace<ec::AiState>(enemy);
    registry.emplace<ec::AiAwareRange>(enemy, 5.f);
    registry.emplace<ec::AiWeapon>(enemy, 20, 31, 99999, 35.f, 0.3);
}

void spawnBullet(entt::registry& registry, const glm::vec2& pos, const float& rot, const float& vel)
{
    namespace ec = ecs::component;
    auto bullet = registry.create();
    registry.emplace<ec::Lifetime>(bullet, ResourceManager::timer->getTotalTime(), 2.0);
    registry.emplace<ec::Position>(bullet, pos);
    registry.emplace<ec::Size>(bullet, glm::vec2{0.1f, 0.1f});
    registry.emplace<ec::Velocity>(bullet, vel);
    registry.emplace<ec::MaxVelocity>(bullet, vel);
    registry.emplace<ec::Acceleration>(bullet, -1.f);
    registry.emplace<ec::Rotation>(bullet, rot);
};

}  // namespace ecs::action
