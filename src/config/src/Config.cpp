#include "config/Config.hpp"

#include <inicpp.h>
#include <spdlog/async.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/spdlog.h>

#include "config/priv/DevUiExtension.hpp"
#include "mono/dev_ui/DevUI.hpp"
#include "mono/log/Logging.hpp"

namespace
{

void initTemporaryLogger()
{
    mono::config::priv::buffered_sink = std::make_shared<mono::log::priv::BufferedSink>();
    spdlog::set_default_logger(
        std::make_shared<spdlog::logger>("buffer", mono::config::priv::buffered_sink));
}

void flushTemporaryLoggerOnFail()
{
    // This is terrible, when single point of initialization is introduced, config::initialize()
    // should return false.
    mono::log::initialize();  // flush happens inside log::initialize() now
    mono::config::priv::buffered_sink.reset();
    mono::config::priv::buffered_sink = nullptr;
}

void shutdownOnFail()
{
    flushTemporaryLoggerOnFail();
    spdlog::shutdown();  // prevents crashes on exit - shuts down thread pool
    std::exit(EXIT_FAILURE);
}

}  // namespace

namespace mono::config
{

void initialize()
{
    ::initTemporaryLogger();

    mono::dev_ui::registerExtension("Settings", mono::config::priv::devUiExtension);
}

void initialize(const std::filesystem::path& path)
{
    ::initTemporaryLogger();

    if(const bool success = runtime.loadFromFile(path); not success)
    {
        spdlog::error("Failed to load config from file: '{}'", path.string());
        ::shutdownOnFail();
    }

    if(const bool success = runtime.validate(); not success)
    {
        spdlog::error("Config validation failed");
        ::shutdownOnFail();
    }

    mono::dev_ui::registerExtension("Settings", mono::config::priv::devUiExtension);
}

}  // namespace mono::config
