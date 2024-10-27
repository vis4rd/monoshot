#pragma once

#include <algorithm>
#include <vector>

#include <inicpp.h>

#include "../ConfigItem.hpp"

namespace mono::config
{

bool optionStringValidator(const std::string& value, const ConfigItemUserData& user_data)
{
    // Requires user data to have options specified as values. Keys are irrelevant.
    if(not user_data)
    {
        return false;
    }

    auto& data = user_data.value();

    std::vector<std::string> options{};
    std::ranges::transform(data, std::back_inserter(options), [](const auto& pair) {
        return pair.second;
    });

    return std::ranges::any_of(options, [&value](const auto& option) {
        return value.compare(option) == 0;
    });
}

}  // namespace mono::config
