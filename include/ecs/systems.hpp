#pragma once

#include "components.hpp"

#include <entt/entity/registry.hpp>

namespace ecs::system
{

namespace impl
{

bool is_colliding_with_anything(entt::registry &registry, const entt::entity &entity, const glm::vec2 &next_pos);

}

void move_bullets(entt::registry &registry);
void collide_with_hero(entt::registry &registry, const entt::entity &hero_id, glm::vec2 &hero_move_direction);

}  // namespace ecs::system
