#pragma once

#include "KeyState.hpp"
#include <functional>
#include <cstdint>

class Keybind
{
    public:
    Keybind() = default;
    Keybind(std::int32_t key, KeyState state, std::function<void()> callback);

    public:
    std::int32_t key;
    KeyState state;
    std::function<void()> callback;
};
