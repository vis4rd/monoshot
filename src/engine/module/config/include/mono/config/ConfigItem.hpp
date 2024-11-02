#pragma once

#include <functional>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>

#include <inicpp.h>

#include "CallbackGuard.hpp"
#include "priv/IniCompliantTrait.hpp"

namespace mono::config
{

using ConfigItemUserData = std::optional<std::map<std::string, std::string>>;
using ConfigItemValidatorFunc = std::function<bool(const std::string&, const ConfigItemUserData&)>;
using ConfigItemSetCallback =
    std::function<void(std::string_view old_value, std::string_view new_value)>;

inline bool defaultValidatorFunc(const std::string& value, const ConfigItemUserData& user_data)
{
    return true;
}

// TODO(vis4rd): Make it a base class
//               1. Require isValid() to be defined
//               2. Require specifying section and key in constructor (through const members I
//                    guess?)
//               3. Require specifying ini storage in constructor
//               4. Remove custom user data storage if possible (all should be accessible in derived
//                  class with normal member fields)
//               5. Figure out how to downcast it in storage safely.
//                  - virtual member function returning unique id
//                    - require derived class to implement it
//                    - probably string would be best
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
    const ConfigItemUserData& getUserData() const;
    bool setValue(const std::string& value);
    bool setValue(const IniEncodableTrait auto& value);

    [[nodiscard]] CallbackGuard setOnSetCallback(ConfigItemSetCallback&& callback);
    void removeOnSetCallback();

    bool isValid() const;

    private:
    const std::string m_section;
    const std::string m_key;
    const ConfigItemUserData m_userData;
    const ConfigItemValidatorFunc m_validator;
    std::optional<ConfigItemSetCallback> m_setCallback;
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
    std::string new_value;
    ini::Convert<std::remove_cvref_t<decltype(value)>>{}.encode(value, new_value);

    return this->setValue(new_value);
}

}  // namespace mono::config
