#include "../../include/gameplay/Hero.hpp"

Hero::Hero(const std::int32_t& health, const std::shared_ptr<Weapon>& main_weapon, const std::shared_ptr<Weapon>& secondary_weapon)
    : health(health),
      maxHealth(health),
      m_mainWeapon(main_weapon),
      m_secondaryWeapon(secondary_weapon)
{
    this->setCurrentItem();
}

void Hero::addItem(const Weapon& weapon)
{
    if(m_mainWeapon == nullptr)
    {
        m_mainWeapon = std::make_shared<Weapon>(weapon);
        currentItem = m_mainWeapon;
    }
    else if(m_secondaryWeapon == nullptr)
    {
        m_secondaryWeapon = std::make_shared<Weapon>(weapon);
    }
}

void Hero::addItem(const Food& food)
{
    m_foodItems.push_back(food);
}

void Hero::dropItem()
{
    if(!currentItem)
    {
        return;
    }

    if(m_mainWeapon == currentItem)
    {
        m_mainWeapon.reset();
        m_mainWeapon = nullptr;
    }
    else if(m_secondaryWeapon == currentItem)
    {
        m_secondaryWeapon.reset();
        m_secondaryWeapon = nullptr;
    }
    else
    {
        m_foodItems.pop_back();
    }

    currentItem.reset();
    currentItem = nullptr;
}

void Hero::setCurrentItem()
{
    if(m_mainWeapon)
    {
        currentItem = m_mainWeapon;
    }
    else if(m_secondaryWeapon)
    {
        currentItem = m_secondaryWeapon;
    }
}
