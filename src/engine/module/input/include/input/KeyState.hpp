#pragma once

enum KeyState
{
    IDLE = 0,
    PRESS_ONCE,
    HOLD,
    RELEASE,
    REPEAT  // currently unused, left for potential future support of callbacks
};
