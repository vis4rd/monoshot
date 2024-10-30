#include "config/ConfigStorage.hpp"

#include <filesystem>

#include <spdlog/spdlog.h>

namespace mono::config
{

ConfigStorage::ConfigStorage(const std::filesystem::path& path)
{
    this->loadFromFile(path);
}

void ConfigStorage::addConfigItem(
    const std::string& section,
    const std::string& key,
    const ConfigItemUserData& user_data)
{
    ConfigItem item{m_iniFile, section, key, user_data};
    m_items.emplace_back(std::move(item));
}

bool ConfigStorage::set(
    const std::string& section,
    const std::string& key,
    const std::string& value)
{
    if(not m_iniFile.contains(section) or not m_iniFile.at(section).contains(key))
    {
        return false;
    }

    const auto result = std::ranges::find_if(m_items, [&](const ConfigItem& item) {
        return (item.getSection().compare(section) == 0) and (item.getKey().compare(key) == 0);
    });
    if(result == m_items.end())
    {
        return false;
    }

    auto opt_value = result->template getValue<std::string>();
    if(not opt_value.has_value())
    {
        return false;
    }

    const auto& prev_value = opt_value.value();
    result->setValue(value);

    if(not result->isValid())
    {
        result->setValue(prev_value);
        return false;
    }

    return true;
}

bool ConfigStorage::validate() const
{
    for(const auto& item : m_items)
    {
        if(not item.isValid())
        {
            return false;
        }
    }
    return true;
}

bool ConfigStorage::loadFromFile(const std::filesystem::path& path)
{
    spdlog::info("Loading config file: {}", path.string());
    if(not std::filesystem::exists(path))
    {
        spdlog::error("Config file '{}' does not exist", path.string());
        return false;
    }

    try
    {
        m_iniFile.load(path.string());
    }
    catch(const std::logic_error& err)
    {
        spdlog::error("Failed to load config file '{}'", err.what());
        return false;
    }

    return true;
}

}  // namespace mono::config
