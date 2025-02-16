#pragma once

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "mono/config/CallbackGuard.hpp"

namespace mono::log::data
{

// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
inline spdlog::source_loc location = spdlog::source_loc::current();
inline std::shared_ptr<spdlog::logger> engine_logger = nullptr;
inline std::shared_ptr<spdlog::logger> app_logger = nullptr;
inline std::optional<config::CallbackGuard> log_level_cb_guard;
// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

}  // namespace mono::log::data
