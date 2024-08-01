#pragma once

#include <cstdint>
#include <format>

#include <glad/gl.h>
#include <spdlog/spdlog.h>

namespace mono::log
{

namespace priv
{

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
inline spdlog::source_loc location = spdlog::source_loc::current();

}  // namespace priv

template<typename... ARGS>
void setGlObjectLabel(
    GLenum identifier,
    GLuint object,
    const std::format_string<ARGS...>& label,
    ARGS&&... args);
void setGlLogLocation(const std::source_location& location = std::source_location::current());
void enableOpenGlLogging();

template<typename... ARGS>
void setGlObjectLabel(
    GLenum identifier,
    GLuint object,
    const std::format_string<ARGS...>& label,
    ARGS&&... args)
{
    const std::string label_str = std::format(label, std::forward<ARGS>(args)...);
    setGlLogLocation();
    glObjectLabel(identifier, object, static_cast<GLsizei>(label_str.size()), label_str.data());
}

}  // namespace mono::log
