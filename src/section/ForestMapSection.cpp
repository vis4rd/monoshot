#include "../../include/section/ForestMapSection.hpp"

ForestMapSection::ForestMapSection()
    : GameplaySection()
{
    m_name = "ForestMapSection";

    m_map.setTheme(MapThemes::FOREST_THEME);
    m_map.loadFromFile("../res/maps/forest.msmap", m_enemyRegistry);
    m_map.addTilesToRegistry(m_mapElementsRegistry);

    m_hero.addItem(std::move(Weapon(10, 31, 76, 35.f, 0.3)));
    m_hero.addItem(std::move(Weapon(10, 70, 20000, 10.f, 0.05)));
}
