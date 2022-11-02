#include "../../include/items/Consumable.hpp"

#include "../../include/utility/Timer.hpp"

void Consumable::use()
{
    if(!this->canBeUsed())
    {
        return;
    }

    m_lastUseTimestamp = Timer::getTotalTime();
}

constexpr bool Consumable::canBeUsed(double timestamp)
{
    if(timestamp == 0.0)
    {
        timestamp = Timer::getTotalTime();
    }
    return m_lastUseTimestamp + m_useDelay < timestamp;
}
