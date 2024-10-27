#pragma once

#include <inicpp.h>

#include "../ConfigItem.hpp"

namespace mono::config
{

bool stringValidator(const std::string&, const ConfigItemUserData&)
{
    return true;
}

}  // namespace mono::config
