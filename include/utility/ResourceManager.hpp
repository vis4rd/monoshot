#pragma once

#include "../window/Window.hpp"

#include <memory>

struct ResourceManager
{
    static std::shared_ptr<Window> window;
};
