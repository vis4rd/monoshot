#include "../include/log/Logging.hpp"

#include <chrono>
#include <filesystem>
#include <fstream>

#include <spdlog/async_logger.h>
#include <spdlog/spdlog.h>

#include "config/Config.hpp"
#include "mono/util/Compiler.hpp"

namespace mono::log
{

constexpr const char *glSourceToStr(GLenum source)
{
    switch(source)
    {
        case GL_DEBUG_SOURCE_API: return "API";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "Window System";
        case GL_DEBUG_SOURCE_SHADER_COMPILER: return "Shader Compiler";
        case GL_DEBUG_SOURCE_THIRD_PARTY: return "Third Party";
        case GL_DEBUG_SOURCE_APPLICATION: return "Application";
        case GL_DEBUG_SOURCE_OTHER: return "Other";
        default: return "Unknown";
    }
}

constexpr const char *glTypeToStr(GLenum type)
{
    switch(type)
    {
        case GL_DEBUG_TYPE_ERROR: return "Error";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "Deprecated Behavior";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "Undefined Behavior";
        case GL_DEBUG_TYPE_PORTABILITY: return "Portability";
        case GL_DEBUG_TYPE_PERFORMANCE: return "Performance";
        case GL_DEBUG_TYPE_MARKER: return "Marker";
        case GL_DEBUG_TYPE_PUSH_GROUP: return "Push Group";
        case GL_DEBUG_TYPE_POP_GROUP: return "Pop Group";
        case GL_DEBUG_TYPE_OTHER: return "Other";
        default: return "Unknown";
    }
}

constexpr spdlog::level glSeverityToSpdlogLevel(GLenum severity)
{
    switch(severity)
    {
        case GL_DEBUG_SEVERITY_HIGH: return spdlog::level::critical;
        case GL_DEBUG_SEVERITY_MEDIUM: return spdlog::level::err;
        case GL_DEBUG_SEVERITY_LOW: return spdlog::level::warn;
        case GL_DEBUG_SEVERITY_NOTIFICATION: return spdlog::level::trace;
        default: return spdlog::level::off;
    }
}

void openGlDebugMessageCallback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const char *message,
    const void *user_param)
{
    spdlog::log(
        data::location,
        glSeverityToSpdlogLevel(severity),
        "\b[#{}][{}: {}] {}",
        id,
        glTypeToStr(type),
        glSourceToStr(source),
        message);
}

void setGlLogLocation(const std::source_location &location)
{
    // line number + 1 because the caller is the one that is interesting
    data::location =
        spdlog::source_loc{location.file_name(), location.line() + 1, location.function_name()};
}

void enableOpenGlLogging()
{
    if(glad_glDebugMessageControl != nullptr)
    {
        // 131169 - ???
        // 131185 - ???
        // 131218 - ???
        // 131204 - texture does not have a defined base level so not generating mipmaps
        std::vector<std::uint32_t> useless_codes = {/*131169, 131185, 131218, */ 131204u};
        useless_codes.shrink_to_fit();

        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        glDebugMessageControl(
            GL_DEBUG_SOURCE_API,
            GL_DEBUG_TYPE_OTHER,
            GL_DONT_CARE,
            static_cast<std::int32_t>(useless_codes.size()),
            useless_codes.data(),
            GL_FALSE);
        glDebugMessageCallback(static_cast<GLDEBUGPROC>(openGlDebugMessageCallback), nullptr);
    }
}

static std::chrono::system_clock::time_point getLocalTime()
{
    if constexpr(mono::util::isGnuCompiler())
    {
        const std::time_t now =
            std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::tm local_tm{};
        ::localtime_s(&local_tm, &now);

        return std::chrono::sys_days{
                   std::chrono::year{local_tm.tm_year + 1900} / (local_tm.tm_mon + 1)
                   / (local_tm.tm_mday)}
               + std::chrono::hours{local_tm.tm_hour} + std::chrono::minutes{local_tm.tm_min}
               + std::chrono::seconds{local_tm.tm_sec};
    }
    else
    {
        // For some reason, clang crashes when checking local timezone (?)
        // so current UTC time is returned instead.
        return std::chrono::system_clock::now();
    }
}

static std::string buildPattern()
{
    // formatting spec: https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
    const bool is_debug = mono::config::runtime::logLevel == spdlog::level::debug;
    const std::string_view debug_thread_file = is_debug ? "[t:%=5!t][%s:%#]" : "";
    const std::string_view debug_time_precision = is_debug ? "%f" : "%e";
    return std::format(
        "[%Y-%m-%d %T.{}][%^%=5!l%$][%=6n]{} %v",
        debug_time_precision,
        debug_thread_file);
}

static std::vector<spdlog::sink_ptr> buildSinks(std::chrono::system_clock::time_point local_time)
{
    const auto formatted_time = std::format("{:%F_%H-%M-%S}", local_time).substr(0, 19);
    const auto file_name_current = std::format("../logs/{}.log", formatted_time);
    const std::string file_name_latest = "../logs/latest.log";

    {
        // create log files
        std::fstream f1{file_name_current};
        std::fstream f2{file_name_latest};
    }

    auto current_file_sink =
        std::make_shared<spdlog::sinks::basic_file_sink_mt>(file_name_current, true);
    auto latest_file_sink =
        std::make_shared<spdlog::sinks::basic_file_sink_mt>(file_name_latest, true);
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    return {console_sink, current_file_sink, latest_file_sink};
}

void initialize()
{
    namespace fs = std::filesystem;
    fs::create_directory("../logs");

    const auto engine_pattern = buildPattern();
    const auto app_pattern = buildPattern();

    const auto local_time = getLocalTime();
    const auto engine_sinks = buildSinks(local_time);
    const auto app_sinks = buildSinks(local_time);

    spdlog::init_thread_pool(8192, 1, []() {
        spdlog::info("Starting logging thread");
    });
    data::app_logger = std::make_shared<spdlog::async_logger>(
        "app",
        app_sinks.begin(),
        app_sinks.end(),
        spdlog::thread_pool(),
        spdlog::async_overflow_policy::overrun_oldest);
    data::app_logger->set_level(mono::config::runtime::logLevel);
    data::app_logger->set_pattern(app_pattern);
    spdlog::register_logger(data::app_logger);

    data::engine_logger = std::make_shared<spdlog::async_logger>(
        "engine",
        engine_sinks.begin(),
        engine_sinks.end(),
        spdlog::thread_pool(),
        spdlog::async_overflow_policy::overrun_oldest);
    data::engine_logger->set_level(mono::config::runtime::logLevel);
    data::engine_logger->set_pattern(engine_pattern);
    spdlog::register_logger(data::engine_logger);

    spdlog::set_default_logger(data::engine_logger);
    spdlog::debug("Logging initialized");
    spdlog::info("Start timestamp: {:%F %T}", local_time);
}

}  // namespace mono::log
