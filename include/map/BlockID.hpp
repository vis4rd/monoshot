#pragma once

#include <cstdint>

enum BlockID : std::size_t
{
    FIRST_BLOCK = 0,

    // write blocks here
    Wall,
    InvisibleWall,

    LAST_BLOCK,
    BLOCK_COUNT = LAST_BLOCK - FIRST_BLOCK - 1
};

inline const char* blockToString(std::size_t block_id)
{
    switch(block_id)
    {
        case BlockID::Wall: return "Wall"; break;
        case BlockID::InvisibleWall: return "InvisibleWall"; break;
        default: return "Unknown block"; break;
    }
}
