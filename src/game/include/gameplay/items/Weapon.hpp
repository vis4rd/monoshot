#pragma once

#include "Consumable.hpp"

class Weapon : public Consumable
{
    public:
    Weapon() = default;
    Weapon(const Weapon& copy);
    Weapon(Weapon&& move);
    Weapon(
        const std::int32_t& damage,
        const std::uint32_t& magazine_capacity,
        const std::uint32_t& total_ammo,
        const float& bullet_velocity,
        const double& use_delay);
    ~Weapon() override = default;

    Weapon& operator=(const Weapon& copy);
    Weapon& operator=(Weapon&& move);

    void reload();
    virtual const std::int32_t& getDamage() const final;
    virtual const std::uint32_t& getAmmoCurrent() const final;
    virtual const std::uint32_t& getAmmoMagazineMax() const final;
    virtual const std::uint32_t& getAmmoTotal() const final;
    virtual const float& getBulletVelocity() const final;

    virtual void setDamage(const std::int32_t& damage) final;
    virtual void setAmmoCurrent(const std::uint32_t& current_ammo) final;
    virtual void setAmmoMagazineMax(const std::uint32_t& magazine_capacity) final;
    virtual void setAmmoTotal(const std::uint32_t& total_ammo) final;
    virtual void setBulletVelocity(const float& velocity) final;

    protected:
    void use() override;

    protected:
    std::int32_t m_damage = 10;
    std::uint32_t m_ammoCurrent = 0;  // current ammo in magazine
    std::uint32_t m_ammoMagazineMax = 30;  // max ammo in magazine
    std::uint32_t m_ammoTotal = 120;  // max ammo in total
    float m_bulletVelocity = 100.f;
};
