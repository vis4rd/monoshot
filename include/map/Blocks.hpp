#pragma once

#include <cstdint>

enum Block : std::size_t
{
    Wall = 0,
    BLOCK_COUNT
};

inline const char* blockToString(std::size_t block)
{
    switch(block)
    {
        case Block::Wall: return "Wall"; break;
        default: return "Unknown block"; break;
    }
}
