#pragma once

#include <inicpp.h>

#include "ConfigStorage.hpp"
#include "priv/StaticConfiguration.hpp"

namespace mono::config
{

namespace data
{

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
inline ConfigStorage configStorage{"../config/config.ini"};

}  // namespace data

void initialize();

}  // namespace mono::config
