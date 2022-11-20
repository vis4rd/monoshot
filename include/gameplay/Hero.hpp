#pragma once

#include "items/Food.hpp"
#include "items/Weapon.hpp"
#include "../texture/Animation.hpp"

#include <cstdint>
#include <memory>
#include <array>
#include <optional>
#include <concepts>

#include <glm/glm.hpp>

namespace impl
{

template<typename T>
concept IsConsumableC = std::derived_from<T, Consumable>;

}

class Hero final
{
    public:
    Hero(const std::int32_t& health, const std::optional<Weapon>& main_weapon = std::nullopt, const std::optional<Weapon>& secondary_weapon = std::nullopt);
    ~Hero();

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
    bool hasItem(const std::size_t& id) const;

    Texture::Animation& getTexture();

    public:
    const std::int32_t maxHealth;
    const float maxVelocity = 7.f;
    const glm::vec2 size = {1.f, 1.f};
    const float acceleration = 70.f;

    std::int32_t health;
    glm::vec2 position = {0.f, 0.f};
    float velocity = 0.f;
    float rotation = 0.f;
    glm::vec2 walkingDirection = {0.f, 0.f};

    private:
    std::array<std::shared_ptr<Consumable>, 3> m_items;
    std::array<bool, 3> m_availableItems;
    std::size_t m_currentItem = 3;

    Texture::Animation m_texture;
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
