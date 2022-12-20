#include "../../include/ecs/actions.hpp"

#include "../../include/ecs/components.hpp"

namespace ecs::action
{


void spawn_enemy(entt::registry& registry, const glm::vec2& pos, const glm::vec2& size, const float& rot, const std::int32_t& health)
{
    auto enemy = registry.create();
    registry.emplace<ecs::component::position>(enemy, pos);
    registry.emplace<ecs::component::size>(enemy, size);
    registry.emplace<ecs::component::rotation>(enemy, rot);
    registry.emplace<ecs::component::health>(enemy, health);
}

void spawn_bullet(entt::registry& registry, const glm::vec2& pos, const float& rot, const float& vel)
{
    auto bullet = registry.create();
    registry.emplace<ecs::component::lifetime>(bullet, ResourceManager::timer->getTotalTime(), 2.0);
    registry.emplace<ecs::component::position>(bullet, pos);
    registry.emplace<ecs::component::size>(bullet, glm::vec2{0.1f, 0.1f});
    registry.emplace<ecs::component::velocity>(bullet, vel);
    registry.emplace<ecs::component::max_velocity>(bullet, vel);
    registry.emplace<ecs::component::acceleration>(bullet, -1.f);
    registry.emplace<ecs::component::rotation>(bullet, rot);
};

}  // namespace ecs::action
