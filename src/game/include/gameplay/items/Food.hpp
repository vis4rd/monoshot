#pragma once

#include "Consumable.hpp"

class Food : public Consumable
{
    public:
    Food();
    Food(const Food& copy) = default;
    Food(Food&& move) noexcept;
    ~Food() override = default;

    Food& operator=(const Food& copy) = default;
    Food& operator=(Food&& move) noexcept;

    virtual const std::int32_t& getHealingPower() const final;

    protected:
    void use() override;

    protected:
    std::int32_t m_healingPower = 1;
};
