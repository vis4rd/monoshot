#include "../include/config/Config.hpp"

#include <memory>

#include <spdlog/spdlog.h>

namespace mono::config
{

bool loadFromFile(const std::filesystem::path& path)
{
    spdlog::info("Loading config file: {}", path.string());
    if(not std::filesystem::exists(path))
    {
        spdlog::error("Config file '{}' does not exist", path.string());
        return false;
    }

    try
    {
        data::iniFile.load(path.string());
    }
    catch(const std::logic_error& err)
    {
        spdlog::error("Failed to load config file '{}'", err.what());
        return false;
    }

    return true;
}

void updateRuntimeConfiguration()
{
    config::runtime::useOpenGl = data::iniFile.at("engine").at("UseOpenGL").as<bool>();
    config::runtime::logLevel =
        spdlog::level_from_str(data::iniFile.at("engine").at("LogLevel").as<std::string>());
}

void initialize()
{
    if(bool success = loadFromFile("../config/config.ini"); success)
    {
        updateRuntimeConfiguration();
    }
}

}  // namespace mono::config
