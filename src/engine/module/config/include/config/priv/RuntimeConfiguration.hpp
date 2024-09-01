#pragma once

#include <spdlog/spdlog.h>

namespace mono::config::runtime
{

// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
inline bool useOpenGl = true;
inline spdlog::level logLevel = spdlog::level::info;
// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

}  // namespace mono::config::runtime
