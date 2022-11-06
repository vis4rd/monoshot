#pragma once

#include "items/Food.hpp"
#include "items/Weapon.hpp"

#include <cstdint>
#include <memory>
#include <array>
#include <optional>
#include <concepts>

namespace impl
{

template<typename T>
concept IsConsumableC = std::derived_from<T, Consumable>;

}

class Hero
{
    public:
    Hero(const std::int32_t& health, const std::optional<Weapon>& main_weapon = std::nullopt, const std::optional<Weapon>& secondary_weapon = std::nullopt);

    template<impl::IsConsumableC T>
    void addItem(T&& item);

    void dropCurrentItem();

    template<impl::IsConsumableC T>
    T& getCurrentItem();

    bool setCurrentItem(const std::size_t& index);
    const std::size_t& getCurrentItemIndex() const;

    bool isInventoryEmpty() const;
    bool holdsWeapon() const;
    bool holdsFood() const;

    public:
    std::int32_t health;
    const std::int32_t maxHealth;

    private:
    std::array<std::shared_ptr<Consumable>, 3> m_items;
    std::array<bool, 3> m_availableItems;
    std::size_t m_currentItem = 3;
};

template<impl::IsConsumableC T>
void Hero::addItem(T&& item)
{
    const auto set_item = [&ai = m_availableItems, &i = m_items, &ci = m_currentItem]<impl::IsConsumableC P>(P&& item, std::size_t index) -> bool
    {
        if(!ai.at(index))
        {
            i.at(index) = std::make_shared<T>(std::move(item));
            ai.at(index) = true;
            if(ci == 3)
            {
                ci = index;
            }
            return true;
        }
        return false;
    };

    if constexpr(std::is_same_v<T, Food>)
    {
        set_item(std::move(item), 2);
    }
    else if constexpr(std::is_same_v<T, Weapon>)
    {
        if(!set_item(std::forward<Weapon>(item), 0))
        {
            set_item(std::move(item), 1);
        }
    }
}

template<impl::IsConsumableC T>
T& Hero::getCurrentItem()
{
    return dynamic_cast<T&>(*(m_items.at(m_currentItem)));
}
