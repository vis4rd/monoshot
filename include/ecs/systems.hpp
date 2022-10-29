#pragma once

#include "components.hpp"

#include <entt/entity/registry.hpp>

namespace ecs::system
{

void move_based_on_rotation(entt::registry &registry, const float &delta_time);
void move(entt::registry &registry, const glm::vec2 &direction, const float &delta_time);
void collide_with_hero(entt::registry &registry, const entt::entity &hero_id, glm::vec2 &hero_move_direction);

}  // namespace ecs::system
