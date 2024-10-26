#pragma once

#include <filesystem>

#include <inicpp.h>

#include "priv/RuntimeConfiguration.hpp"
#include "priv/StaticConfiguration.hpp"

namespace mono::config
{

namespace data
{

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
inline ini::IniFile iniFile{};

}  // namespace data

void initialize();

}  // namespace mono::config
