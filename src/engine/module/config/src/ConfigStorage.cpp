#include "config/ConfigStorage.hpp"

#include <filesystem>

#include <spdlog/spdlog.h>

namespace mono::config
{

ConfigStorage::ConfigStorage(const std::filesystem::path& path)
{
    this->loadFromFile(path);
}

bool ConfigStorage::validate() const
{
    for(const auto& item : m_items)
    {
        if(not item->isValid())
        {
            return false;
        }
    }
    return true;
}

bool ConfigStorage::loadFromFile(const std::filesystem::path& path)
{
    spdlog::info("Loading config file: {}", path.string());
    if(not std::filesystem::exists(path))
    {
        spdlog::error("Config file '{}' does not exist", path.string());
        return false;
    }

    try
    {
        m_iniFile.load(path.string());
    }
    catch(const std::logic_error& err)
    {
        spdlog::error("Failed to load config file '{}'", err.what());
        return false;
    }

    return true;
}

}  // namespace mono::config
