#include "../../../include/gameplay/items/Consumable.hpp"

#include "../../include/utility/Timer.hpp"
#include <spdlog/spdlog.h>

void Consumable::use()
{
    m_lastUseTimestamp = Timer::getTotalTime();
}

bool Consumable::canBeUsed(double timestamp)
{
    if(std::abs(timestamp) < 1.0e-4)
    {
        timestamp = Timer::getTotalTime();
    }
    return (m_lastUseTimestamp + m_useDelay) < timestamp;
}
