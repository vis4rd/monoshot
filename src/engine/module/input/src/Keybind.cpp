#include "../include/input/Keybind.hpp"

Keybind::Keybind(std::int32_t key, KeyState state, std::function<void()> callback)
    : key(key)
    , state(state)
    , callback(callback)
{ }
