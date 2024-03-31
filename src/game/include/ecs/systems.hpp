#pragma once

#include <entt/entity/registry.hpp>

#include "../gameplay/Hero.hpp"
#include "components.hpp"

namespace ecs::system
{

void moveBullets(entt::registry &registry);
void checkAliveBullets(entt::registry &registry);
/**
 * @brief Remove all entities from given registry that own a 'destroyed' component
 *
 * @param registry
 */
void destroyEntities(entt::registry &registry);
void moveHero(entt::registry &registry, Hero &hero, glm::vec2 &hero_move_direction);
void updateAis(
    entt::registry &enemy_registry,
    const glm::vec2 &hero_pos,
    entt::registry &bullet_registry);

}  // namespace ecs::system
