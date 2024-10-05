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
    // engine
    const auto engine_section = data::iniFile.at("engine");
    config::runtime::useOpenGl = engine_section.at("UseOpenGL").as<bool>();
    config::runtime::logLevel =
        spdlog::level_from_str(engine_section.at("LogLevel").as<std::string>());

    // engine.window
    const auto engine_window_section = data::iniFile.at("engine.window");
    config::runtime::windowMode = engine_window_section.at("Mode").as<type::WindowMode>();
    config::runtime::useVSync = engine_window_section.at("UseVSync").as<bool>();
    config::runtime::resolution =
        engine_window_section.at("Resolution").as<type::WindowResolution>();
}

void initialize()
{
    if(bool success = loadFromFile("../config/config.ini"); success)
    {
        updateRuntimeConfiguration();
    }
}

}  // namespace mono::config
