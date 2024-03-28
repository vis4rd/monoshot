#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "opengl/target/RenderWindow.hpp"
#include "opengl/texture/Texture.hpp"
#include "time/FramerateLimiter.hpp"
#include "time/Timer.hpp"
#include "ui/Font.hpp"
// #include "window/Window.hpp"

class Timer;
class FramerateLimiter;

struct ResourceManager
{
    // NOLINTBEGIN(readability-identifier-naming,cppcoreguidelines-avoid-non-const-global-variables)
    static std::shared_ptr<mono::gl::RenderWindow> window;
    static std::shared_ptr<Timer> timer;
    static glm::vec4 mapThemeBackgroundColor;

    static std::shared_ptr<mono::Texture> carTexture;
    static std::shared_ptr<mono::Texture> destroyedCarTexture;
    static std::shared_ptr<mono::Texture> chairTexture;
    static std::shared_ptr<mono::Texture> outdoorBenchTexture;
    static std::shared_ptr<mono::Texture> tableTexture;
    static std::shared_ptr<mono::Texture> smallTreeTexture;
    static std::shared_ptr<mono::Texture> largeTreeTexture;
    static std::shared_ptr<mono::Texture> smallBushTexture;
    static std::shared_ptr<mono::Texture> largeBushTexture;

    static std::shared_ptr<mono::Texture> rifleInventoryTexture;
    static std::shared_ptr<mono::Texture> pistolInventoryTexture;

    static std::shared_ptr<mono::Texture> enemyTexture;

    static std::shared_ptr<Font> uiTitleFont;
    static std::shared_ptr<float> uiTitleFontSize;
    static std::shared_ptr<Font> uiButtonFont;
    static std::shared_ptr<float> uiButtonFontSize;
    static std::shared_ptr<Font> uiAmmoFont;
    static std::shared_ptr<float> uiAmmoFontSize;

    static std::shared_ptr<FramerateLimiter> framerateLimiter;
    // NOLINTEND(readability-identifier-naming,cppcoreguidelines-avoid-non-const-global-variables)
};
