#include "config/Config.hpp"

#include <inicpp.h>
#include <spdlog/spdlog.h>

#include "config/types/BasicConfigItem.hpp"
#include "config/types/OptionStringConfigItem.hpp"

namespace mono::config
{

void initialize()
{
    runtime.addConfigItem<BasicConfigItem<bool>>(std::string{"engine"}, std::string{"UseOpenGL"});
    runtime.addConfigItem<OptionStringConfigItem>(
        std::string{"engine"},
        std::string{"LogLevel"},
        std::vector<std::string>{"trace", "debug", "info", "warn", "error", "critical"});

    runtime.addConfigItem<OptionStringConfigItem>(
        std::string{"engine.window"},
        std::string{"Mode"},
        std::vector<std::string>{"windowed", "fullscreen", "borderless"});
    runtime.addConfigItem<BasicConfigItem<bool>>("engine.window", "UseVSync");
    runtime.addConfigItem<MultiNumberConfigItem<2, std::int32_t, 'x'>>(
        "engine.window",
        "Resolution");
}

}  // namespace mono::config
