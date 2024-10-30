#pragma once

#include <functional>
#include <map>
#include <optional>
#include <string>
#include <string_view>

#include <inicpp.h>

#include "priv/IniCompliantTrait.hpp"

namespace mono::config
{

using ConfigItemUserData = std::optional<std::map<std::string, std::string>>;
using ConfigItemValidatorFunc = std::function<bool(const std::string&, const ConfigItemUserData&)>;

inline bool defaultValidatorFunc(const std::string& value, const ConfigItemUserData& user_data)
{
    return true;
}

class ConfigItem final
{
    public:
    ConfigItem(
        ini::IniFile& ini_storage,
        const std::string& section,
        const std::string& key,
        const ConfigItemUserData& user_data = std::nullopt,
        ConfigItemValidatorFunc validator = defaultValidatorFunc);

    std::string_view getSection() const;
    std::string_view getKey() const;
    template<typename NATIVE_TYPE>
    std::optional<NATIVE_TYPE> getValue() const;
    bool setValue(const std::string& value);
    bool setValue(const IniEncodableTrait auto& value);

    bool isValid() const;

    private:
    const std::string m_section;
    const std::string m_key;
    const ConfigItemUserData m_userData;
    const ConfigItemValidatorFunc m_validator;
    ini::IniFile& m_iniStorage;
};

template<typename NATIVE_TYPE>
std::optional<NATIVE_TYPE> ConfigItem::getValue() const
{
    if(not m_iniStorage.contains(m_section))
    {
        return std::nullopt;
    }

    const auto& section = m_iniStorage.at(m_section);
    if(not section.contains(m_key))
    {
        return std::nullopt;
    }

    return section.at(m_key).as<NATIVE_TYPE>();
}

bool ConfigItem::setValue(const IniEncodableTrait auto& value)
{
    if(not m_iniStorage.contains(m_section))
    {
        return false;
    }

    std::string str_value;
    ini::Convert<decltype(value)>{}.encode(value, str_value);
    m_iniStorage.at(m_section).at(m_key) = str_value;
    return true;
}

}  // namespace mono::config
