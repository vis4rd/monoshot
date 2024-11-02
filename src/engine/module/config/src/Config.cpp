#include "config/Config.hpp"

#include <spdlog/spdlog.h>

#include "config/validators/BooleanValidator.hpp"
#include "config/validators/OptionStringValidator.hpp"

namespace mono::config
{

void initialize()
{
    runtime.addConfigItem("engine", "UseOpenGL", std::nullopt, booleanValidator);
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
    },
        optionStringValidator);

    runtime.addConfigItem(
        "engine.window",
        "Mode",
        ConfigItemUserData{
            {{"o1", "windowed"}, {"o2", "fullscreen"}, {"o3", "borderless"}}
    },
        optionStringValidator);
    runtime.addConfigItem("engine.window", "UseVSync", std::nullopt, booleanValidator);
    runtime.addConfigItem("engine.window", "Resolution");
}

}  // namespace mono::config
