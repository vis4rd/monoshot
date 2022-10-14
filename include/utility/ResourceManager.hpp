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

    static std::shared_ptr<Font> uiTitleFont;
    static std::shared_ptr<float> uiTitleFontSize;
    static std::shared_ptr<Font> uiButtonFont;
    static std::shared_ptr<float> uiButtonFontSize;
};
