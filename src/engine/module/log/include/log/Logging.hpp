#pragma once

#include <cstdint>

#include <spdlog/spdlog.h>

namespace util
{

constexpr const char *logSourceStr(std::uint32_t source);
constexpr const char *logTypeStr(std::uint32_t type);
void openGLDebugMessageCallback(
    std::uint32_t source,
    std::uint32_t type,
    std::uint32_t id,
    std::uint32_t severity,
    std::int32_t length,
    const char *message,
    const void *user_param);
void enableOpenGlLogging();

}  // namespace util
