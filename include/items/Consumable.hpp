#pragma once

#include "Item.hpp"

class Consumable : public Item
{
    public:
    Consumable() = default;
    virtual ~Consumable() = default;

    void use() override = 0;

    protected:
    constexpr bool canBeUsed(double timestamp = 0.0);

    protected:
    double m_useDelay = 0.1;  // in seconds

    private:
    double m_lastUseTimestamp = 0.0;  // if m_lastUseTimestamp + m_useDelay < Timer::getTotalTime() then it can be used again
};
