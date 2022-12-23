#include "../../include/section/WinterMapSection.hpp"

WinterMapSection::WinterMapSection()
    : GameplaySection()
{
    m_name = "WinterMapSection";

    m_map.setTheme(MapThemes::WINTER_THEME);
    m_map.loadFromFile("../res/maps/winter.msmap", m_enemyRegistry);
    m_map.addTilesToRegistry(m_mapElementsRegistry);

    m_hero.addItem(std::move(Weapon(10, 31, 76, 35.f, 0.3)));
    m_hero.addItem(std::move(Weapon(10, 70, 20000, 10.f, 0.05)));
}
