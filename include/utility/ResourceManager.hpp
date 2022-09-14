#pragma once

#include "../window/Window.hpp"
#include "../utility/Timer.hpp"

#include <memory>

struct ResourceManager
{
    static std::shared_ptr<Window> window;
    static std::shared_ptr<Timer> timer;
};
