#pragma once

#include <cstdint>

enum class KeyState : std::uint8_t
{
    IDLE = 0,
    PRESS_ONCE,
    HOLD,
    RELEASE,
    REPEAT  // currently unused, left for potential future support of callbacks
};
