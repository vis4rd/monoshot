#include "config/Config.hpp"

#include <inicpp.h>
#include <spdlog/async.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/spdlog.h>

#include "config/types/BasicConfigItem.hpp"
#include "config/types/OptionStringConfigItem.hpp"
#include "mono/dev_ui/DevUI.hpp"
#include "mono/log/Logging.hpp"

namespace mono::config
{

static void initTemporaryLogger()
{
    priv::buffered_sink = std::make_shared<log::priv::BufferedSink>();
    spdlog::set_default_logger(std::make_shared<spdlog::logger>("buffer", priv::buffered_sink));
}

static void flushTemporaryLoggerOnFail()
{
    // This is terrible, when single point of initialization is introduced, config::initialize()
    // should return false.
    mono::log::initialize();  // flush happens inside log::initialize() now
    priv::buffered_sink.reset();
    priv::buffered_sink = nullptr;
}

void initialize()
{
    initTemporaryLogger();
    runtime.loadFromFile("../config/config.ini");

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
        flushTemporaryLoggerOnFail();
        spdlog::shutdown();  // prevents crashes on exit - shuts down thread pool
        std::exit(EXIT_FAILURE);
    }

    mono::dev_ui::registerWindow("Settings");
}

}  // namespace mono::config
