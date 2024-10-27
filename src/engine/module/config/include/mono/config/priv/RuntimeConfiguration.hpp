#pragma once

#include <spdlog/spdlog.h>

#include "CustomFields.hpp"

namespace mono::config::runtime
{

// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)

// engine
inline bool useOpenGl;
inline spdlog::level logLevel;

// engine.window
inline type::WindowMode windowMode = type::WindowMode::FULLSCREEN;
inline bool useVSync;
inline type::WindowResolution resolution;

// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

}  // namespace mono::config::runtime
