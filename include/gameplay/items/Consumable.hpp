#pragma once

#include "Item.hpp"

class Consumable : public Item
{
    public:
    Consumable() = default;
    Consumable(const Consumable& copy);
    Consumable(Consumable&& move);
    virtual ~Consumable() = default;

    Consumable& operator=(const Consumable& copy);
    Consumable& operator=(Consumable&& move);

    virtual bool useDelayed() final;

    protected:
    virtual void use() override;
    virtual bool canBeUsed(double timestamp = 0.0) final;

    virtual const double& getUseDelay() const final;
    virtual const double& getLastUseTimestamp() const final;
    virtual void setUseDelay(const double& delay) final;

    private:
    double m_useDelay = 0.1;  // in seconds
    double m_lastUseTimestamp = 0.0;  // if m_lastUseTimestamp + m_useDelay < Timer::getTotalTime() then it can be used again
};
