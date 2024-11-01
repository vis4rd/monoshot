#include "config/Config.hpp"

#include <spdlog/spdlog.h>

namespace mono::config
{

void initialize()
{
    runtime.addConfigItem("engine", "UseOpenGL");
    runtime.addConfigItem(
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

    runtime.addConfigItem(
        "engine.window",
        "Mode",
        ConfigItemUserData{
            {{"o1", "windowed"}, {"o2", "fullscreen"}, {"o3", "borderless"}}
    });
    runtime.addConfigItem("engine.window", "UseVSync");
    runtime.addConfigItem("engine.window", "Resolution");
}

}  // namespace mono::config
