#include "config/ConfigItem.hpp"

#include <spdlog/spdlog.h>

namespace mono::config
{

ConfigItem::ConfigItem(
    ini::IniFile& ini_storage,
    const std::string& section,
    const std::string& key,
    const ConfigItemUserData& user_data,
    ConfigItemValidatorFunc validator)
    : m_section(section)
    , m_key(key)
    , m_userData(user_data)
    , m_validator(std::move(validator))
    , m_iniStorage(ini_storage)
{ }

std::string_view ConfigItem::getSection() const
{
    return m_section;
}

std::string_view ConfigItem::getKey() const
{
    return m_key;
}

bool ConfigItem::setValue(const std::string& value)
{
    if(not m_iniStorage.contains(m_section))
    {
        return false;
    }

    auto& field = m_iniStorage.at(m_section).at(m_key);
    const auto old_value = field.as<std::string>();

    spdlog::info(
        "Setting ConfigItem[{}][{}] to '{}' (was '{}')",
        m_section,
        m_key,
        value,
        old_value);
    field = value;

    if(m_setCallback.has_value())
    {
        spdlog::debug("Invoking callback for ConfigItem[{}][{}]", m_section, m_key);
        std::invoke(m_setCallback.value(), old_value, value);
    }
    return true;
}

const ConfigItemUserData& ConfigItem::getUserData() const
{
    return m_userData;
}

CallbackGuard ConfigItem::setOnSetCallback(ConfigItemSetCallback&& callback)
{
    spdlog::debug("Setting OnSet callback for ConfigItem[{}][{}]", m_section, m_key);
    m_setCallback = std::move(callback);
    return CallbackGuard{*this};
}

void ConfigItem::removeOnSetCallback()
{
    spdlog::debug("Removing OnSet callback for ConfigItem[{}][{}]", m_section, m_key);
    m_setCallback.reset();
}

bool ConfigItem::isValid() const
{
    return std::invoke(m_validator, this->getValue<std::string>().value_or(""), m_userData);
}

}  // namespace mono::config
