#pragma once

#include "KeyState.hpp"

class Keybind
{
    public:
    Keybind() = default;
    Keybind(int32_t key, int32_t scancode, KeyState state, std::function<void()> callback);

    public:
    int32_t key;
    int32_t scancode;
    KeyState required_state;
    KeyState previous_state = KeyState::IDLE;
    KeyState current_state = KeyState::IDLE;
    std::function<void()> callback;
};
