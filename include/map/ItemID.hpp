#pragma once

#include <cstdint>

enum ItemID : std::size_t
{
    FIRST_ITEM = 1000,

    // write items here
    MedKit,
    Bandage,

    LAST_ITEM,
    ITEM_COUNT = LAST_ITEM - FIRST_ITEM - 1
};

inline const char* itemIdToString(std::size_t item_id)
{
    switch(item_id)
    {
        case ItemID::MedKit: return "MedKit"; break;
        case ItemID::Bandage: return "Bandage"; break;
        default: return "Unknown item"; break;
    }
}
