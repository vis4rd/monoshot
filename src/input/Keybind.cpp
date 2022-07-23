#include "../../include/input/Keybind.hpp"

Keybind::Keybind(int32_t key, int32_t scancode, KeyState state, std::function<void()> callback)
    : key(key), scancode(scancode), required_state(state), callback(callback)
{
}
