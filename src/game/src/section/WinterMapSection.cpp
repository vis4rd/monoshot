#include "../../include/section/WinterMapSection.hpp"

WinterMapSection::WinterMapSection()
    : GameplaySection()
{
    m_name = "WinterMapSection";

    m_map.setTheme(MapThemes::WINTER_THEME);
    m_map.loadFromFile("../res/maps/winter.msmap", m_enemyRegistry);
    m_map.addTilesToRegistry(m_mapElementsRegistry);

    {  // rifle
        constexpr std::int32_t damage = 20;
        constexpr std::uint32_t magazine_capacity = 78;
        constexpr std::uint32_t total_ammo = 441;
        constexpr float bullet_velocity = 27.f;
        constexpr double use_delay = 0.07;  // in seconds
        m_hero.addItem(
            std::move(Weapon(damage, magazine_capacity, total_ammo, bullet_velocity, use_delay)));
    }

    {  // pistol
        constexpr std::int32_t damage = 20;
        constexpr std::uint32_t magazine_capacity = 13;
        constexpr std::uint32_t total_ammo = 52;
        constexpr float bullet_velocity = 30.f;
        constexpr double use_delay = 0.30;  // in seconds
        m_hero.addItem(
            std::move(Weapon(damage, magazine_capacity, total_ammo, bullet_velocity, use_delay)));
    }
}
