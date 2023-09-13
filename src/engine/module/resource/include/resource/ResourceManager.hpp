#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "texture/Texture.hpp"
#include "time/FramerateLimiter.hpp"
#include "time/Timer.hpp"
#include "ui/Font.hpp"
#include "window/Window.hpp"

class Timer;
class FramerateLimiter;

struct ResourceManager
{
    static std::shared_ptr<mono::Window> window;
    static std::shared_ptr<Timer> timer;
    static glm::vec4 mapThemeBackgroundColor;

    static Texture::Texture carTexture;
    static Texture::Texture destroyedCarTexture;
    static Texture::Texture chairTexture;
    static Texture::Texture outdoorBenchTexture;
    static Texture::Texture tableTexture;
    static Texture::Texture smallTreeTexture;
    static Texture::Texture largeTreeTexture;
    static Texture::Texture smallBushTexture;
    static Texture::Texture largeBushTexture;

    static Texture::Texture rifleInventoryTexture;
    static Texture::Texture pistolInventoryTexture;

    static Texture::Texture enemyTexture;

    static std::shared_ptr<Font> uiTitleFont;
    static std::shared_ptr<float> uiTitleFontSize;
    static std::shared_ptr<Font> uiButtonFont;
    static std::shared_ptr<float> uiButtonFontSize;
    static std::shared_ptr<Font> uiAmmoFont;
    static std::shared_ptr<float> uiAmmoFontSize;

    static std::shared_ptr<FramerateLimiter> framerateLimiter;
};
