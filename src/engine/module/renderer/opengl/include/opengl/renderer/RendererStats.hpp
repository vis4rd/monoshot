#pragma once

#include <cstdint>

namespace mono
{

class RendererStats
{
    public:
    void clear()
    {
        drawCalls = 0;
        geometryUpdateSize = 0;
    }

    public:
    std::uint32_t drawCalls{};
    std::uint32_t geometryUpdateSize{};
};

}  // namespace mono
