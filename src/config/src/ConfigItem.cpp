#include "config/ConfigItem.hpp"

#include <imgui/imgui.h>
#include <spdlog/spdlog.h>

namespace mono::config
{

ConfigItem::ConfigItem(
    ini::IniFile& ini_storage,
    const std::string& section,
    const std::string& key)
    : m_section(section)
    , m_key(key)
    , m_iniStorage(ini_storage)
{ }

// NOLINTNEXTLINE(modernize-use-equals-default)
ConfigItem& ConfigItem::operator=(const ConfigItem& copy)
{
    m_section = copy.m_section;
    m_key = copy.m_key;
    m_iniStorage = copy.m_iniStorage;
    m_setCallback = copy.m_setCallback;
    return *this;
}

ConfigItem& ConfigItem::operator=(ConfigItem&& move) noexcept
{
    m_section = std::move(move.m_section);
    m_key = std::move(move.m_key);
    m_iniStorage = move.m_iniStorage;
    m_setCallback = std::move(move.m_setCallback);
    return *this;
}

bool ConfigItem::isValid() const
{
    return true;
}

std::string ConfigItem::getType() const
{
    return "ConfigItem";
}

void ConfigItem::drawForDevUi()
{
    ImGui::Text("%s - <draw func not specified>", m_key.c_str());
}

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

std::string ConfigItem::getNativeValue() const
{
    return m_iniStorage.at(m_section).at(m_key).as<std::string>();
}

}  // namespace mono::config
