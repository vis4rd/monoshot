#include "../../include/utility/ResourceManager.hpp"

std::shared_ptr<Window> ResourceManager::window;
std::shared_ptr<Timer> ResourceManager::timer;
glm::vec4 ResourceManager::mapThemeBackgroundColor;
std::shared_ptr<Font> ResourceManager::uiTitleFont;
std::shared_ptr<float> ResourceManager::uiTitleFontSize;
std::shared_ptr<Font> ResourceManager::uiButtonFont;
std::shared_ptr<float> ResourceManager::uiButtonFontSize;
