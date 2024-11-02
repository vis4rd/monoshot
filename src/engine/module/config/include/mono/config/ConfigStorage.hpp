#pragma once

#include <filesystem>
#include <vector>

#include "ConfigItem.hpp"

namespace mono::config
{

class ConfigStorage final
{
    public:
    explicit ConfigStorage(const std::filesystem::path& path);

    // TODO(vis4rd): Add support for adding callbacks
    // TODO(vis4rd): Add support for adding item type
    void addConfigItem(
        const std::string& section,
        const std::string& key,
        const ConfigItemUserData& user_data = std::nullopt,
        ConfigItemValidatorFunc validator = defaultValidatorFunc);
    std::optional<std::reference_wrapper<ConfigItem>> get(
        const std::string& section,
        const std::string& key);
    bool validate() const;

    private:
    bool loadFromFile(const std::filesystem::path& path);

    private:
    // TODO(vis4rd): Replace with map that takes section and key as a key
    std::vector<ConfigItem> m_items{};
    ini::IniFile m_iniFile;
};

}  // namespace mono::config
