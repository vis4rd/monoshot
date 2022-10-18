#pragma once

#include "../ui/fonts/Font.hpp"
#include "../utility/Timer.hpp"
#include "../window/Window.hpp"

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

    static std::shared_ptr<Font> uiTitleFont;
    static std::shared_ptr<float> uiTitleFontSize;
    static std::shared_ptr<Font> uiButtonFont;
    static std::shared_ptr<float> uiButtonFontSize;
};
