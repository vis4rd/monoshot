#pragma once

#include <cstdint>

enum BlockID : std::size_t
{
    Wall = 0,
    BLOCK_COUNT
};

inline const char* blockToString(std::size_t block_id)
{
    switch(block_id)
    {
        case BlockID::Wall: return "Wall"; break;
        default: return "Unknown block"; break;
    }
}
