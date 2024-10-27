#pragma once

#include <stdexcept>

#include <inicpp.h>

#include "../ConfigItem.hpp"

namespace mono::config
{

bool booleanValidator(const std::string& value, const ConfigItemUserData& user_data)
{
    bool result{};
    ini::Convert<bool> converter{};

    try
    {
        converter.decode(value, result);
    }
    catch(std::invalid_argument& err)
    {
        return false;
    }
    return true;
}


}  // namespace mono::config
