#pragma once

#include "Grid.hpp"

class Map : public Grid
{
    public:
    Map();
    ~Map() = default;

    void loadFromFile(const std::string_view& filename, const std::string_view& allowed_char = "1234567890abcdefghijklmnopqrstuvwxyz");
};
