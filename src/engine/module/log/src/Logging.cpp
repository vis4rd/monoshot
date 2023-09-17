#include "../include/log/Logging.hpp"

#include <glad/gl.h>

static constexpr const char *sources[] =
    {"API", "Window System", "Shader Compiler", "Third Party", "Application", "Other", "Unknown"};
static constexpr const char *types[] = {
    "Error",
    "Deprecated Behavior",
    "Undefined Behavior",
    "Portability",
    "Performance",
    "Other",
    "Unknown"};

namespace util
{

constexpr const char *logSourceStr(std::uint32_t source)
{
    return sources[std::min(
        source - GL_DEBUG_SOURCE_API,
        static_cast<std::uint32_t>(sizeof(sources) / sizeof(const char *)))];
}

constexpr const char *logTypeStr(std::uint32_t type)
{
    return types[std::min(
        type - GL_DEBUG_TYPE_ERROR,
        static_cast<std::uint32_t>(sizeof(types) / sizeof(const char *)))];
}

void openGLDebugMessageCallback(
    std::uint32_t source,
    std::uint32_t type,
    std::uint32_t id,
    std::uint32_t severity,
    std::int32_t length,
    const char *message,
    const void *userParam)
{
    switch(severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:
            spdlog::critical(
                "[Object: {}][Type: {}][Source: {}] {}",
                id,
                logTypeStr(type),
                logSourceStr(source),
                message);
            return;
        case GL_DEBUG_SEVERITY_MEDIUM:
            spdlog::error(
                "[Object: {}][Type: {}][Source: {}] {}",
                id,
                logTypeStr(type),
                logSourceStr(source),
                message);
            return;
        case GL_DEBUG_SEVERITY_LOW:
            spdlog::warn(
                "[Object: {}][Type: {}][Source: {}] {}",
                id,
                logTypeStr(type),
                logSourceStr(source),
                message);
            return;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            spdlog::trace(
                "[Object: {}][Type: {}][Source: {}] {}",
                id,
                logTypeStr(type),
                logSourceStr(source),
                message);
            return;
    }
}

void enableOpenGlLogging()
{
    if(glDebugMessageControl != nullptr)
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
            useless_codes.size(),
            useless_codes.data(),
            GL_FALSE);
        glDebugMessageCallback((GLDEBUGPROC)openGLDebugMessageCallback, nullptr);
    }
}

}  // namespace util
