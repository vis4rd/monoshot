#pragma once

#include "ItemID.hpp"

class Item
{
    public:
    Item() = default;
    virtual ~Item() = default;

    virtual void use() = 0;

    public:
    const ItemID ID = ItemID::Undefined;
};
