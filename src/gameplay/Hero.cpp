#include "../../include/gameplay/Hero.hpp"

#include <spdlog/spdlog.h>

Hero::Hero(const std::int32_t& health, const std::optional<Weapon>& main_weapon, const std::optional<Weapon>& secondary_weapon)
    : health(health),
      maxHealth(health),
      m_items{nullptr, nullptr, nullptr},
      m_availableItems{false, false, false},
      m_texture{Resource::create<Texture::impl::Animation>("../res/textures/entities/player.png",
          0.1,
          TextureData{
              .widthTotal = 128,
              .heightTotal = 16,
              .widthSub = 16,
              .heightSub = 16,
              .numberOfSubs = 8,
              .numberOfSubsInOneRow = 8,
          })}
{
    if(main_weapon)
    {
        m_items.at(0) = std::make_shared<Weapon>(std::move(main_weapon.value()));
        m_availableItems.at(0) = true;
    }
    if(secondary_weapon)
    {
        m_items.at(1) = std::make_shared<Weapon>(std::move(secondary_weapon.value()));
        m_availableItems.at(1) = true;
    }
    this->setCurrentItem(1);
    this->setCurrentItem(0);
}

Hero::~Hero() { }

void Hero::dropCurrentItem()
{
    if(isInventoryEmpty())
    {
        return;
    }

    if(m_availableItems.at(m_currentItem))
    {
        m_items.at(m_currentItem).reset();
        m_items.at(m_currentItem) = nullptr;
        m_availableItems.at(m_currentItem) = false;
        m_currentItem = 3;
    }

    // select a new current item
    if(m_currentItem == 3)
    {
        for(std::size_t i = 0; i < 3; i++)
        {
            if(m_availableItems.at(i) && (m_items.at(i) != nullptr))
            {
                m_currentItem = i;
                break;
            }
        }
    }
}

bool Hero::isInventoryEmpty() const
{
    const auto& v = m_availableItems;  // abbreviation
    return !(v[0] || v[1] || v[2]);
}

bool Hero::setCurrentItem(const std::size_t& index)
{
    if(m_availableItems.at(index % m_items.size()))
    {
        m_currentItem = (index % m_items.size());
        return true;
    }
    return false;
}

const std::size_t& Hero::getCurrentItemIndex() const
{
    return m_currentItem;
}

bool Hero::holdsWeapon() const
{
    return (m_currentItem < 2) && m_availableItems[m_currentItem];
}

bool Hero::holdsFood() const
{
    return (m_currentItem == 2) && m_availableItems[m_currentItem];
}

bool Hero::hasItem(const std::size_t& id) const
{
    for(std::size_t i = 0; i < m_availableItems.size(); i++)
    {
        if(m_availableItems.at(i) && (m_items.at(i)->ID == id))
        {
            return true;
        }
    }
    return false;
}

Texture::Animation& Hero::getTexture()
{
    return m_texture;
}
