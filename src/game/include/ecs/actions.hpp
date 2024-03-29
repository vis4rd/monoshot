#pragma once

#include <entt/entity/registry.hpp>
#include <glm/glm.hpp>

namespace ecs::action
{

void spawnEnemy(
    entt::registry& registry,
    const glm::vec2& pos,
    const glm::vec2& size = {1.f, 1.f},
    const float& rot = 0.f,
    const std::int32_t& health = 100);
void spawnBullet(
    entt::registry& registry,
    const glm::vec2& pos,
    const float& rot,
    const float& vel);

}  // namespace ecs::action
