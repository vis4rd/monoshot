#pragma once

#include "Consumable.hpp"

class Food : public Consumable
{
    public:
    Food(std::int32_t& entity_health);
    virtual ~Food() = default;

    void use() override;

    protected:
    std::int32_t m_healingPower = 1;
    std::int32_t& m_entityHealth;
};
