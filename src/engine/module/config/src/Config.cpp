#include "config/Config.hpp"

#include <spdlog/spdlog.h>

namespace mono::config
{

void initialize()
{
    data::configStorage.addConfigItem("engine", "UseOpenGL");
    data::configStorage.addConfigItem(
        "engine",
        "LogLevel",
        ConfigItemUserData{
            {{"o1", "trace"},
             {"o2", "debug"},
             {"o3", "info"},
             {"o4", "warn"},
             {"o5", "error"},
             {"o6", "critical"}}
    });

    data::configStorage.addConfigItem(
        "engine.window",
        "Mode",
        ConfigItemUserData{
            {{"o1", "windowed"}, {"o2", "fullscreen"}, {"o3", "borderless"}}
    });
    data::configStorage.addConfigItem("engine.window", "UseVSync");
    data::configStorage.addConfigItem("engine.window", "Resolution");
}

}  // namespace mono::config
