#include "../../include/section/ForestMapSection.hpp"

ForestMapSection::ForestMapSection()
    : GameplaySection()
{
    m_name = "ForestMapSection";

    m_map.setTheme(MapThemes::forestTheme);
    m_map.loadFromFile("../res/maps/forest.msmap", m_enemyRegistry);
    m_map.addTilesToRegistry(m_mapElementsRegistry);

    {  // rifle
        constexpr std::int32_t damage = 20;
        constexpr std::uint32_t magazineCapacity = 31;
        constexpr std::uint32_t totalAmmo = 93;
        constexpr float bulletVelocity = 35.f;
        constexpr double useDelay = 0.15;  // in seconds
        m_hero.addItem(
            std::move(Weapon(damage, magazineCapacity, totalAmmo, bulletVelocity, useDelay)));
    }

    {  // pistol
        constexpr std::int32_t damage = 20;
        constexpr std::uint32_t magazineCapacity = 13;
        constexpr std::uint32_t totalAmmo = 91;
        constexpr float bulletVelocity = 30.f;
        constexpr double useDelay = 0.30;  // in seconds
        m_hero.addItem(
            std::move(Weapon(damage, magazineCapacity, totalAmmo, bulletVelocity, useDelay)));
    }
}
