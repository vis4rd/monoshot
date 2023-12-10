#pragma once

#include <cstdint>

namespace mono
{

struct RendererStats
{
    std::uint32_t drawCount = 0;
    std::int32_t indexCount = 0;
    std::int32_t instanceCount = 0;

    std::uint32_t quadCount = 0;
    std::int32_t lineCount = 0;
};

}  // namespace mono
