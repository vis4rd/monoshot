#pragma once

#include <cstdint>

struct RendererStats
{
    std::uint32_t drawCount = 0;
    std::uint32_t indexCount = 0;

    std::uint32_t quadCount = 0;
    std::uint32_t lineCount = 0;
};
