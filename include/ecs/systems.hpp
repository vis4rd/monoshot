#pragma once

#include "components.hpp"

#include <entt/entity/registry.hpp>

namespace ecs::system
{

void move_bullets(entt::registry &registry);
void collide_with_hero(entt::registry &registry, const entt::entity &hero_id, glm::vec2 &hero_move_direction);

}  // namespace ecs::system
