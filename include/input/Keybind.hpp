#pragma once

#include "KeyState.hpp"

class Keybind
{
    public:
    Keybind() = default;
    Keybind(int32_t key, KeyState state, std::function<void()> callback);

    public:
    int32_t key;
    KeyState state;
    std::function<void()> callback;
};
