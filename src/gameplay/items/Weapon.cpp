#include "../../../include/gameplay/items/Weapon.hpp"

#include <algorithm>
#include <spdlog/spdlog.h>

Weapon::Weapon(const std::int32_t& damage, const std::uint32_t& magazine_capacity, const std::uint32_t& total_ammo, const float& bullet_velocity, const double& use_delay)
    : damage(damage),
      ammoCurrent(0),
      ammoMagazineMax(magazine_capacity),
      ammoTotal(total_ammo),
      bulletVelocity(bullet_velocity)
{
    m_useDelay = use_delay;
}

void Weapon::use()
{
    if(!this->canBeUsed())
    {
        return;
    }
    if(ammoCurrent > 0)
    {
        ammoCurrent--;
    }
    else
    {
        this->reload();
    }
    Consumable::use();
}

void Weapon::reload()
{
    ammoTotal += ammoCurrent;
    ammoCurrent = 0;

    const auto difference = std::min(ammoTotal, ammoMagazineMax);
    ammoTotal -= difference;
    ammoCurrent += difference;
}
