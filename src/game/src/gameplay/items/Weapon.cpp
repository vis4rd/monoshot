#include "../../../include/gameplay/items/Weapon.hpp"

#include <algorithm>

#include <spdlog/spdlog.h>

Weapon::Weapon(
    const std::int32_t& damage,
    const std::uint32_t& magazine_capacity,
    const std::uint32_t& total_ammo,
    const float& bullet_velocity,
    const double& use_delay)
    : m_damage(damage)
    , m_ammoCurrent(0)
    , m_ammoMagazineMax(magazine_capacity)
    , m_ammoTotal(total_ammo)
    , m_bulletVelocity(bullet_velocity)
{
    this->setUseDelay(use_delay);
}

Weapon::Weapon(const Weapon& copy)
    : Consumable(copy)
    , m_damage(copy.m_damage)
    , m_ammoCurrent(copy.m_ammoCurrent)
    , m_ammoMagazineMax(copy.m_ammoMagazineMax)
    , m_ammoTotal(copy.m_ammoTotal)
    , m_bulletVelocity(copy.m_bulletVelocity)
{ }

Weapon::Weapon(Weapon&& move) noexcept
    : Consumable(std::move(move))
    , m_damage(std::move(move.m_damage))
    , m_ammoCurrent(std::move(move.m_ammoCurrent))
    , m_ammoMagazineMax(std::move(move.m_ammoMagazineMax))
    , m_ammoTotal(std::move(move.m_ammoTotal))
    , m_bulletVelocity(std::move(move.m_bulletVelocity))
{ }

Weapon& Weapon::operator=(const Weapon& copy)
{
    m_damage = copy.m_damage;
    m_ammoCurrent = copy.m_ammoCurrent;
    m_ammoMagazineMax = copy.m_ammoMagazineMax;
    m_ammoTotal = copy.m_ammoTotal;
    m_bulletVelocity = copy.m_bulletVelocity;
    this->Consumable::operator=(copy);
    return *this;
}

Weapon& Weapon::operator=(Weapon&& move) noexcept
{
    m_damage = std::move(move.m_damage);
    m_ammoCurrent = std::move(move.m_ammoCurrent);
    m_ammoMagazineMax = std::move(move.m_ammoMagazineMax);
    m_ammoTotal = std::move(move.m_ammoTotal);
    m_bulletVelocity = std::move(move.m_bulletVelocity);
    this->Consumable::operator=(std::move(move));
    return *this;
}

void Weapon::reload()
{
    m_ammoTotal += m_ammoCurrent;
    m_ammoCurrent = 0;

    const auto difference = std::min(m_ammoTotal, m_ammoMagazineMax);
    m_ammoTotal -= difference;
    m_ammoCurrent += difference;
}

const std::int32_t& Weapon::getDamage() const
{
    return m_damage;
}

const std::uint32_t& Weapon::getAmmoCurrent() const
{
    return m_ammoCurrent;
}

const std::uint32_t& Weapon::getAmmoMagazineMax() const
{
    return m_ammoMagazineMax;
}

const std::uint32_t& Weapon::getAmmoTotal() const
{
    return m_ammoTotal;
}

const float& Weapon::getBulletVelocity() const
{
    return m_bulletVelocity;
}

void Weapon::setDamage(const std::int32_t& damage)
{
    m_damage = damage;
}

void Weapon::setAmmoCurrent(const std::uint32_t& current_ammo)
{
    m_ammoCurrent = current_ammo;
}

void Weapon::setAmmoMagazineMax(const std::uint32_t& magazine_capacity)
{
    m_ammoMagazineMax = magazine_capacity;
}

void Weapon::setAmmoTotal(const std::uint32_t& total_ammo)
{
    m_ammoTotal = total_ammo;
}

void Weapon::setBulletVelocity(const float& velocity)
{
    m_bulletVelocity = velocity;
}

void Weapon::use()
{
    if(m_ammoCurrent > 0)
    {
        m_ammoCurrent--;
    }
    else
    {
        this->reload();
    }
}
