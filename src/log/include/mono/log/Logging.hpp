#pragma once

#include <format>
#include <source_location>
#include <string>

#include <glbinding/gl/gl.h>
#include <spdlog/spdlog.h>

#include "priv/LoggingData.hpp"

namespace mono::log
{

template<typename... ARGS>
void setGlObjectLabel(
    ::gl::GLenum identifier,
    ::gl::GLuint object,
    const std::format_string<ARGS...>& label,
    ARGS&&... args);
void setGlLogLocation(const std::source_location& location = std::source_location::current());
void enableOpenGlLogging();
void initialize();

template<typename... ARGS>
void setGlObjectLabel(
    ::gl::GLenum identifier,
    ::gl::GLuint object,
    const std::format_string<ARGS...>& label,
    ARGS&&... args)
{
    const std::string label_str = std::format(label, std::forward<ARGS>(args)...);
    setGlLogLocation();
    glObjectLabel(
        identifier,
        object,
        static_cast<::gl::GLsizei>(label_str.size()),
        label_str.data());
}

void critical(spdlog::loc_with_fmt fmt, auto&&... args)
{
    data::app_logger->critical(fmt, std::forward<decltype(args)>(args)...);
}

void error(spdlog::loc_with_fmt fmt, auto&&... args)
{
    data::app_logger->error(fmt, std::forward<decltype(args)>(args)...);
}

void warning(spdlog::loc_with_fmt fmt, auto&&... args)
{
    data::app_logger->warn(fmt, std::forward<decltype(args)>(args)...);
}

void info(spdlog::loc_with_fmt fmt, auto&&... args)
{
    data::app_logger->info(fmt, std::forward<decltype(args)>(args)...);
}

void debug(spdlog::loc_with_fmt fmt, auto&&... args)
{
    data::app_logger->debug(fmt, std::forward<decltype(args)>(args)...);
}

void trace(spdlog::loc_with_fmt fmt, auto&&... args)
{
    data::app_logger->trace(fmt, std::forward<decltype(args)>(args)...);
}

}  // namespace mono::log
