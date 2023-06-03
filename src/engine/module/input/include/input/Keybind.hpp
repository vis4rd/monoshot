#pragma once

#include <cstdint>
#include <functional>

#include "KeyState.hpp"

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
