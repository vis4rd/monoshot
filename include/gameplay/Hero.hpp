#pragma once

#include "items/Food.hpp"
#include "items/Weapon.hpp"

#include <cstdint>
#include <memory>
#include <vector>

class Hero
{
    public:
    Hero(const std::int32_t& health, const std::shared_ptr<Weapon>& main_weapon = nullptr, const std::shared_ptr<Weapon>& secondary_weapon = nullptr);

    void addItem(const Weapon& weapon);
    void addItem(const Food& food);
    void dropItem();

    private:
    void setCurrentItem();

    public:
    std::int32_t health;
    const std::int32_t maxHealth;
    std::shared_ptr<Item> currentItem = nullptr;

    private:
    std::shared_ptr<Weapon> m_mainWeapon = nullptr;
    std::shared_ptr<Weapon> m_secondaryWeapon = nullptr;
    std::vector<Food> m_foodItems = {};
};
