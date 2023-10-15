#pragma once

#include <string>

#include "Keybind.hpp"

class KeyGroup
{
    public:
    KeyGroup() = default;
    explicit KeyGroup(const std::string& name);

    public:
    std::string name{};
    std::unordered_map<std::int32_t, Keybind> keybinds{};
};
