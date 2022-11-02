#include "../../include/items/Food.hpp"

Food::Food(std::int32_t& entity_health)
    : m_entityHealth(entity_health)
{
    m_useDelay = 3.0;
}

void Food::use()
{
    Consumable::use();
    m_entityHealth += m_healingPower;
}
