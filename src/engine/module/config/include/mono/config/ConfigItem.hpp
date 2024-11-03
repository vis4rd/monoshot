#pragma once

#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>

#include <inicpp.h>

#include "CallbackGuard.hpp"
#include "priv/IniCompliantTrait.hpp"

namespace mono::config
{

using ConfigItemSetCallback =
    std::function<void(std::string_view old_value, std::string_view new_value)>;

class ConfigItem
{
    public:
    ConfigItem(ini::IniFile& ini_storage, const std::string& section, const std::string& key);
    ConfigItem(const ConfigItem&) = default;
    ConfigItem(ConfigItem&&) = default;
    virtual ~ConfigItem() = default;

    ConfigItem& operator=(const ConfigItem& copy);
    ConfigItem& operator=(ConfigItem&& move) noexcept;

    virtual bool isValid() const = 0;
    virtual constexpr std::string_view getType() const = 0;

    std::string_view getSection() const;
    std::string_view getKey() const;
    template<typename NATIVE_TYPE>
    std::optional<NATIVE_TYPE> getValue() const;
    bool setValue(const std::string& value);
    bool setValue(const IniEncodableTrait auto& value);

    [[nodiscard]] CallbackGuard setOnSetCallback(ConfigItemSetCallback&& callback);
    void removeOnSetCallback();

    protected:
    std::string getNativeValue() const;

    private:
    std::string m_section;
    std::string m_key;
    ini::IniFile& m_iniStorage;
    std::optional<ConfigItemSetCallback> m_setCallback;
};

constexpr std::string_view ConfigItem::getType() const
{
    return "ConfigItem";
}

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
