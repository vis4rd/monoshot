#include "../include/log/Logging.hpp"

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
        priv::location,
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
    priv::location =
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

}  // namespace mono::log
