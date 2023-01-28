#pragma once

#include "Keybind.hpp"
#include <string>

class KeyGroup
{
    public:
    KeyGroup() = default;
    KeyGroup(const std::string& name);

    public:
    std::string name{};
    std::unordered_map<std::int32_t, Keybind> keybinds{};
};
