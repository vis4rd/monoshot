#include "../../include/utility/Logging.hpp"

#include <glad/glad.h>

static constexpr const char *sources[] = {"API", "Window System", "Shader Compiler", "Third Party", "Application", "Other", "Unknown"};
static constexpr const char *types[] = {"Error", "Deprecated Behavior", "Undefined Behavior", "Portability", "Performance", "Other", "Unknown"};

namespace util
{

constexpr const char *logSourceStr(std::uint32_t source)
{
    return sources[std::min(source - GL_DEBUG_SOURCE_API, static_cast<std::uint32_t>(sizeof(sources) / sizeof(const char *)))];
}

constexpr const char *logTypeStr(std::uint32_t type)
{
    return types[std::min(type - GL_DEBUG_TYPE_ERROR, static_cast<std::uint32_t>(sizeof(types) / sizeof(const char *)))];
}

void openGLDebugMessageCallback(std::uint32_t source, std::uint32_t type, std::uint32_t id, std::uint32_t severity, std::int32_t length, const char *message, const void *userParam)
{
    switch(severity)
    {
        case GL_DEBUG_SEVERITY_HIGH: spdlog::critical("[Object: {}][Type: {}][Source: {}] {}", id, logTypeStr(type), logSourceStr(source), message); return;
        case GL_DEBUG_SEVERITY_MEDIUM: spdlog::error("[Object: {}][Type: {}][Source: {}] {}", id, logTypeStr(type), logSourceStr(source), message); return;
        case GL_DEBUG_SEVERITY_LOW: spdlog::warn("[Object: {}][Type: {}][Source: {}] {}", id, logTypeStr(type), logSourceStr(source), message); return;
        case GL_DEBUG_SEVERITY_NOTIFICATION: spdlog::debug("[Object: {}][Type: {}][Source: {}] {}", id, logTypeStr(type), logSourceStr(source), message); return;
    }
}

void enableOpenGlLogging()
{
    if(glDebugMessageControl != nullptr)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
        glDebugMessageCallback((GLDEBUGPROC)openGLDebugMessageCallback, nullptr);
    }
}

}  // namespace util
