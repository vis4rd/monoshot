#pragma once

#include "Grid.hpp"

template<std::size_t WIDTH, std::size_t HEIGHT>
class Map : public Grid<WIDTH, HEIGHT>
{
    public:
    Map();
    ~Map() = default;

    void loadFromFile(const std::string_view& filename, const std::string_view& allowed_char = "1234567890abcdefghijklmnopqrstuvwxyz");
};

#include "../../src/map/Map.inl"
