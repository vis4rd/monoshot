#pragma once

#include "ItemID.hpp"

class Item
{
    public:
    Item() = default;
    Item(const Item& copy) = default;
    Item(Item&& copy) = default;
    virtual ~Item() = default;

    Item& operator=(const Item& copy) { return *this; }

    Item& operator=(Item&& move) { return *this; }

    protected:
    virtual void use() = 0;

    public:
    const ItemID ID = ItemID::Undefined;
};
