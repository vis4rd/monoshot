#pragma once

#include "../ui/fonts/Font.hpp"
#include "../utility/Timer.hpp"
#include "../window/Window.hpp"
#include "../texture/Animation.hpp"

#include <memory>
#include <entt/entity/entity.hpp>

struct ResourceManager
{
    static std::shared_ptr<Window> window;
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
    static Texture::Animation heroTexture;

    static std::shared_ptr<Font> uiTitleFont;
    static std::shared_ptr<float> uiTitleFontSize;
    static std::shared_ptr<Font> uiButtonFont;
    static std::shared_ptr<float> uiButtonFontSize;
    static std::shared_ptr<Font> uiAmmoFont;
    static std::shared_ptr<float> uiAmmoFontSize;

    static std::shared_ptr<entt::entity> heroEntity;
};
