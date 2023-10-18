#include "../../../include/gameplay/items/Consumable.hpp"

#include <cmath>

#include <time/Timer.hpp>

Consumable::Consumable(Consumable&& move) noexcept
    : Item(std::move(move))
    , m_useDelay(move.m_useDelay)
    , m_lastUseTimestamp(move.m_lastUseTimestamp)
{ }

Consumable& Consumable::operator=(Consumable&& move) noexcept
{
    m_useDelay = move.m_useDelay;
    m_lastUseTimestamp = move.m_lastUseTimestamp;
    this->Item::operator=(std::move(move));
    return *this;
}

bool Consumable::useDelayed()
{
    if(!this->canBeUsed())
    {
        return false;
    }
    this->use();
    m_lastUseTimestamp = Timer::getTotalTime();
    return true;
}

void Consumable::use()
{
    return;
}

bool Consumable::canBeUsed(double timestamp)
{
    if(std::abs(timestamp) < 1.0e-4)
    {
        timestamp = Timer::getTotalTime();
    }
    return (m_lastUseTimestamp + m_useDelay) < timestamp;
}

const double& Consumable::getUseDelay() const
{
    return m_useDelay;
}

const double& Consumable::getLastUseTimestamp() const
{
    return m_lastUseTimestamp;
}

void Consumable::setUseDelay(const double& delay)
{
    m_useDelay = delay;
}
