#pragma once

#include <cstdint>

enum ItemID : std::size_t
{
    FirstAidKit = 0,
    ITEM_COUNT
};

inline const char* itemIdToString(std::size_t item_id)
{
    switch(item_id)
    {
        case ItemID::FirstAidKit: return "First Aid Kit"; break;
        default: return "Unknown item"; break;
    }
}
