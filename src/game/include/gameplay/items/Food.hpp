#pragma once

#include "Consumable.hpp"

class Food : public Consumable
{
    public:
    Food();
    Food(const Food& copy);
    Food(Food&& move);
    virtual ~Food() = default;

    Food& operator=(const Food& copy);
    Food& operator=(Food&& move);

    virtual const std::int32_t& getHealingPower() const final;

    protected:
    void use() override;

    protected:
    std::int32_t m_healingPower = 1;
};