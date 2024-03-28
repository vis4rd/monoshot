#include "../include/resource/ResourceManager.hpp"

// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
std::shared_ptr<mono::gl::RenderWindow> ResourceManager::window;
std::shared_ptr<Timer> ResourceManager::timer;
glm::vec4 ResourceManager::mapThemeBackgroundColor;

std::shared_ptr<mono::Texture> ResourceManager::carTexture;
std::shared_ptr<mono::Texture> ResourceManager::destroyedCarTexture;
std::shared_ptr<mono::Texture> ResourceManager::chairTexture;
std::shared_ptr<mono::Texture> ResourceManager::outdoorBenchTexture;
std::shared_ptr<mono::Texture> ResourceManager::tableTexture;
std::shared_ptr<mono::Texture> ResourceManager::smallTreeTexture;
std::shared_ptr<mono::Texture> ResourceManager::largeTreeTexture;
std::shared_ptr<mono::Texture> ResourceManager::smallBushTexture;
std::shared_ptr<mono::Texture> ResourceManager::largeBushTexture;

std::shared_ptr<mono::Texture> ResourceManager::rifleInventoryTexture;
std::shared_ptr<mono::Texture> ResourceManager::pistolInventoryTexture;

std::shared_ptr<mono::Texture> ResourceManager::enemyTexture;

std::shared_ptr<Font> ResourceManager::uiTitleFont;
std::shared_ptr<float> ResourceManager::uiTitleFontSize;
std::shared_ptr<Font> ResourceManager::uiButtonFont;
std::shared_ptr<float> ResourceManager::uiButtonFontSize;
std::shared_ptr<Font> ResourceManager::uiAmmoFont;
std::shared_ptr<float> ResourceManager::uiAmmoFontSize;

std::shared_ptr<FramerateLimiter> ResourceManager::framerateLimiter;
// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
