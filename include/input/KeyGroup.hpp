#pragma once

#include "Keybind.hpp"

class KeyGroup
{
    public:
    KeyGroup() = default;
    KeyGroup(const std::string& name);

    public:
    std::string name{};
    std::unordered_map<int32_t, Keybind> keybinds{};
};