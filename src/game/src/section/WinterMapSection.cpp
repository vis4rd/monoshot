#include "../../include/section/WinterMapSection.hpp"

WinterMapSection::WinterMapSection()
    : GameplaySection()
{
    m_name = "WinterMapSection";

    m_map.setTheme(MapThemes::winterTheme);
    m_map.loadFromFile("../res/maps/winter.msmap", m_enemyRegistry);
    m_map.addTilesToRegistry(m_mapElementsRegistry);

    {  // rifle
        constexpr std::int32_t damage = 20;
        constexpr std::uint32_t magazineCapacity = 78;
        constexpr std::uint32_t totalAmmo = 441;
        constexpr float bulletVelocity = 27.f;
        constexpr double useDelay = 0.07;  // in seconds
        m_hero.addItem(
            std::move(Weapon(damage, magazineCapacity, totalAmmo, bulletVelocity, useDelay)));
    }

    {  // pistol
        constexpr std::int32_t damage = 20;
        constexpr std::uint32_t magazineCapacity = 13;
        constexpr std::uint32_t totalAmmo = 52;
        constexpr float bulletVelocity = 30.f;
        constexpr double useDelay = 0.30;  // in seconds
        m_hero.addItem(
            std::move(Weapon(damage, magazineCapacity, totalAmmo, bulletVelocity, useDelay)));
    }
}
