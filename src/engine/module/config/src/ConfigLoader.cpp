#include "../include/config/ConfigLoader.hpp"

#include <memory>

#include <spdlog/spdlog.h>

#include "../include/config/RuntimeConfiguration.hpp"

namespace mono
{

ConfigLoader& ConfigLoader::get()
{
    static ConfigLoader instance{};
    return instance;
}

bool ConfigLoader::loadFromFile(const std::filesystem::path& path)
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

ConfigLoader::ConfigLoader()
{
    if(bool success = this->loadFromFile("../config/config.ini"); success)
    {
        this->updateRuntimeConfiguration();
    }
}

void ConfigLoader::updateRuntimeConfiguration() const
{
    config::runtime::UseOpenGL = m_iniFile.at("engine").at("UseOpenGL").as<bool>();
}

}  // namespace mono
