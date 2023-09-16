#include "../../include/section/ForestMapSection.hpp"

ForestMapSection::ForestMapSection()
    : GameplaySection()
{
    m_name = "ForestMapSection";

    m_map.setTheme(MapThemes::FOREST_THEME);
    m_map.loadFromFile("../res/maps/forest.msmap", m_enemyRegistry);
    m_map.addTilesToRegistry(m_mapElementsRegistry);

    {  // rifle
        constexpr std::int32_t damage = 20;
        constexpr std::uint32_t magazine_capacity = 31;
        constexpr std::uint32_t total_ammo = 93;
        constexpr float bullet_velocity = 35.f;
        constexpr double use_delay = 0.15;  // in seconds
        m_hero.addItem(
            std::move(Weapon(damage, magazine_capacity, total_ammo, bullet_velocity, use_delay)));
    }

    {  // pistol
        constexpr std::int32_t damage = 20;
        constexpr std::uint32_t magazine_capacity = 13;
        constexpr std::uint32_t total_ammo = 91;
        constexpr float bullet_velocity = 30.f;
        constexpr double use_delay = 0.30;  // in seconds
        m_hero.addItem(
            std::move(Weapon(damage, magazine_capacity, total_ammo, bullet_velocity, use_delay)));
    }
}
