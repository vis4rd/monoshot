#include "config/ConfigItem.hpp"

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

bool ConfigItem::isValid() const
{
    return std::invoke(m_validator, this->getValue<std::string>().value_or(""), m_userData);
}

}  // namespace mono::config
