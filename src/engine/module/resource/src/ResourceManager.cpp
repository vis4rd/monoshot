#include "../include/utility/ResourceManager.hpp"

std::shared_ptr<Window> ResourceManager::window;
std::shared_ptr<Timer> ResourceManager::timer;
glm::vec4 ResourceManager::mapThemeBackgroundColor;

Texture::Texture ResourceManager::carTexture;
Texture::Texture ResourceManager::destroyedCarTexture;
Texture::Texture ResourceManager::chairTexture;
Texture::Texture ResourceManager::outdoorBenchTexture;
Texture::Texture ResourceManager::tableTexture;
Texture::Texture ResourceManager::smallTreeTexture;
Texture::Texture ResourceManager::largeTreeTexture;
Texture::Texture ResourceManager::smallBushTexture;
Texture::Texture ResourceManager::largeBushTexture;

Texture::Texture ResourceManager::rifleInventoryTexture;
Texture::Texture ResourceManager::pistolInventoryTexture;

Texture::Texture ResourceManager::enemyTexture;

std::shared_ptr<Font> ResourceManager::uiTitleFont;
std::shared_ptr<float> ResourceManager::uiTitleFontSize;
std::shared_ptr<Font> ResourceManager::uiButtonFont;
std::shared_ptr<float> ResourceManager::uiButtonFontSize;
std::shared_ptr<Font> ResourceManager::uiAmmoFont;
std::shared_ptr<float> ResourceManager::uiAmmoFontSize;

std::shared_ptr<FramerateLimiter> ResourceManager::framerateLimiter;
