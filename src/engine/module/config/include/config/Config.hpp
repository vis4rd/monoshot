#pragma once

#include <filesystem>

#include <inicpp.h>

#include "priv/RuntimeConfiguration.hpp"
#include "priv/StaticConfiguration.hpp"

namespace mono::config
{

namespace data
{
inline ini::IniFile iniFile{};
}

void initialize();

}  // namespace mono::config
