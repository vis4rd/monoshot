#pragma once

#include "Consumable.hpp"

class Weapon : public Consumable
{
    public:
    Weapon() = default;
    Weapon(const std::int32_t& damage, const std::uint32_t& magazine_capacity, const std::uint32_t& total_ammo, const float& bullet_velocity, const float& use_delay);
    virtual ~Weapon() = default;

    void use() override;
    void reload();

    public:
    std::int32_t damage = 10;
    std::uint32_t ammoCurrent = 0;  // current ammo in magazine
    std::uint32_t ammoMagazineMax = 30;  // max ammo in magazine
    std::uint32_t ammoTotal = 120;  // max ammo in total
    float bulletVelocity = 100.f;
};
