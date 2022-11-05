#include "../../../include/gameplay/items/Consumable.hpp"

#include "../../include/utility/Timer.hpp"

Consumable::Consumable(const Consumable& copy)
    : Item(copy),
      m_useDelay(copy.m_useDelay),
      m_lastUseTimestamp(copy.m_lastUseTimestamp)
{
}

Consumable::Consumable(Consumable&& move)
    : Item(std::move(move)),
      m_useDelay(std::move(move.m_useDelay)),
      m_lastUseTimestamp(std::move(move.m_lastUseTimestamp))
{
}

Consumable& Consumable::operator=(const Consumable& copy)
{
    m_useDelay = copy.m_useDelay;
    m_lastUseTimestamp = copy.m_lastUseTimestamp;
    this->Item::operator=(copy);
    return *this;
}

Consumable& Consumable::operator=(Consumable&& move)
{
    m_useDelay = std::move(move.m_useDelay);
    m_lastUseTimestamp = std::move(move.m_lastUseTimestamp);
    this->Item::operator=(std::move(move));
    return *this;
}

void Consumable::useDelayed()
{
    if(!this->canBeUsed())
    {
        return;
    }
    this->use();
    m_lastUseTimestamp = Timer::getTotalTime();
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
