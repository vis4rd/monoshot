#pragma once

#include <inicpp.h>

#include "ConfigStorage.hpp"
#include "priv/StaticConfiguration.hpp"
#include "priv/converters/GlmVec2Converter.hpp"
#include "priv/converters/SpdlogLevelConverter.hpp"

namespace mono::config
{

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
inline ConfigStorage runtime{"../config/config.ini"};

void initialize();

}  // namespace mono::config
