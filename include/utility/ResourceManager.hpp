#pragma once

#include "../window/Window.hpp"
#include "../utility/Timer.hpp"

#include <memory>

struct ResourceManager
{
    static std::shared_ptr<Window> window;
    static std::shared_ptr<Timer> timer;
    static glm::vec4 mapThemeBackgroundColor;
    static std::shared_ptr<Texture2D> carTexture;
    static std::shared_ptr<Texture2D> destroyedCarTexture;
    static std::shared_ptr<Texture2D> chairTexture;
    static std::shared_ptr<Texture2D> outdoorBenchTexture;
    static std::shared_ptr<Texture2D> tableTexture;
    static std::shared_ptr<Texture2D> smallTreeTexture;
    static std::shared_ptr<Texture2D> largeTreeTexture;
    static std::shared_ptr<Texture2D> smallBushTexture;
    static std::shared_ptr<Texture2D> largeBushTexture;
};
