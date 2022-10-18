#include "../../include/utility/ResourceManager.hpp"

std::shared_ptr<Window> ResourceManager::window;
std::shared_ptr<Timer> ResourceManager::timer;
glm::vec4 ResourceManager::mapThemeBackgroundColor;
std::shared_ptr<Texture2D> ResourceManager::carTexture;
std::shared_ptr<Texture2D> ResourceManager::destroyedCarTexture;
std::shared_ptr<Texture2D> ResourceManager::chairTexture;
std::shared_ptr<Texture2D> ResourceManager::outdoorBenchTexture;
std::shared_ptr<Texture2D> ResourceManager::tableTexture;
std::shared_ptr<Texture2D> ResourceManager::smallTreeTexture;
std::shared_ptr<Texture2D> ResourceManager::largeTreeTexture;
std::shared_ptr<Texture2D> ResourceManager::smallBushTexture;
std::shared_ptr<Texture2D> ResourceManager::largeBushTexture;
