#pragma once

#include "components.hpp"
#include "../gameplay/Hero.hpp"

#include <entt/entity/registry.hpp>

namespace ecs::system
{

namespace impl
{

bool is_colliding_with_anything(entt::registry &registry, const glm::vec2 &entity_size, const glm::vec2 &next_pos);

}

void move_bullets(entt::registry &registry);
void destroy_bullets(entt::registry &registry);
void move_hero_with_collisions(entt::registry &registry, Hero &hero, glm::vec2 &hero_move_direction);

}  // namespace ecs::system
