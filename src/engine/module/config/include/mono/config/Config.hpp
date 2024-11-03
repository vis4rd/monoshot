#pragma once

#include <inicpp.h>

#include "ConfigStorage.hpp"
#include "priv/StaticConfiguration.hpp"  // IWYU pragma: export
#include "priv/converters/GlmVec2Converter.hpp"  // IWYU pragma: export
#include "priv/converters/SpdlogLevelConverter.hpp"  // IWYU pragma: export

namespace mono::config
{

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
inline ConfigStorage runtime{"../config/config.ini"};

void initialize();

}  // namespace mono::config
