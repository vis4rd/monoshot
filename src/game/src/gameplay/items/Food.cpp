#include "../../../include/gameplay/items/Food.hpp"

#include <utility>

Food::Food()
{
    this->setUseDelay(3.0);
}

Food::Food(Food&& move) noexcept
    : Consumable(std::move(move))
    , m_healingPower(move.m_healingPower)
{ }

Food& Food::operator=(Food&& move) noexcept
{
    m_healingPower = move.m_healingPower;
    this->Consumable::operator=(std::move(move));
    return *this;
}

const std::int32_t& Food::getHealingPower() const
{
    return m_healingPower;
}

void Food::use()
{
    Consumable::use();
}
