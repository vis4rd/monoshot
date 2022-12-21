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
void check_alive_bullets(entt::registry &registry);
void collide_bullets(entt::registry &bullet_registry, entt::registry &map_registry, entt::registry &enemy_registry);
/**
 * @brief Remove all entities from given registry that own a 'destroyed' component
 *
 * @param registry
 */
void destroy_entities(entt::registry &registry);
void move_hero_with_collisions(entt::registry &registry, Hero &hero, glm::vec2 &hero_move_direction);
void update_ais(entt::registry &enemy_registry, const glm::vec2 &hero_pos, entt::registry &bullet_registry);

}  // namespace ecs::system
