#pragma once

#include <algorithm>
#include <vector>

#include "ConfigItem.hpp"

namespace mono::config
{

class ConfigStorage final
{
    public:
    explicit ConfigStorage(const std::filesystem::path& path);

    void addConfigItem(
        const std::string& section,
        const std::string& key,
        const ConfigItemUserData& user_data = std::nullopt);
    template<typename NATIVE_TYPE>
    std::optional<NATIVE_TYPE> get(const std::string& section, const std::string& key) const;
    bool validate() const;

    private:
    bool loadFromFile(const std::filesystem::path& path);

    private:
    // TODO(vis4rd): Replace with map that takes section and key as a key
    std::vector<ConfigItem> m_items{};
    ini::IniFile m_iniFile;
};

template<typename NATIVE_TYPE>
std::optional<NATIVE_TYPE> ConfigStorage::get(const std::string& section, const std::string& key)
    const
{
    const auto result = std::ranges::find_if(m_items, [&](const ConfigItem& item) {
        return (item.getSection().compare(section) == 0) and (item.getKey().compare(key) == 0);
    });
    if(result == m_items.end())
    {
        return std::nullopt;
    }

    return result->template getValue<NATIVE_TYPE>();
}

}  // namespace mono::config
