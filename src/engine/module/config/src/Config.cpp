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

    const auto success = runtime.validate();
    if(not success)
    {
        spdlog::error("Config validation failed");
        // BUG: The program will exit before dumping logs to the latest.log file, so without console
        //      the user will not know what went wrong.
        // TODO(vis4rd): Implement some form of graceful shutdown with logging for this case.
        std::exit(EXIT_FAILURE);
    }
}

}  // namespace mono::config
