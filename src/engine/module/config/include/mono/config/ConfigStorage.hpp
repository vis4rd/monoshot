#pragma once

#include <concepts>
#include <filesystem>
#include <vector>

#include <inicpp.h>

#include "ConfigItem.hpp"
#include "types/BasicConfigItem.hpp"  // IWYU pragma: keep
#include "types/MultiNumberConfigItem.hpp"  // IWYU pragma: keep
#include "types/OptionStringConfigItem.hpp"  // IWYU pragma: keep

namespace mono::config
{

class ConfigStorage final
{
    public:
    explicit ConfigStorage(const std::filesystem::path& path);

    template<std::derived_from<ConfigItem> T>
    T& addConfigItem(auto&&... args)
    requires std::constructible_from<T, ini::IniFile&, decltype(args)...>;

    template<std::derived_from<ConfigItem> T>
    std::optional<std::reference_wrapper<T>> get(
        const std::string& section,
        const std::string& key);
    bool validate() const;
    auto begin();
    auto end();
    auto begin() const;
    auto end() const;

    private:
    bool loadFromFile(const std::filesystem::path& path);

    private:
    // TODO(vis4rd): Replace with map that takes section and key as a key
    std::vector<std::unique_ptr<ConfigItem>> m_items{};
    ini::IniFile m_iniFile;
};

template<std::derived_from<ConfigItem> T>
T& ConfigStorage::addConfigItem(auto&&... args)
requires std::constructible_from<T, ini::IniFile&, decltype(args)...>
{
    std::unique_ptr<ConfigItem> item =
        std::make_unique<T>(m_iniFile, std::forward<decltype(args)>(args)...);
    m_items.push_back(std::move(item));
    return static_cast<T&>(*m_items.back());
}

template<std::derived_from<ConfigItem> T>
std::optional<std::reference_wrapper<T>> ConfigStorage::get(
    const std::string& section,
    const std::string& key)
{
    auto result = std::ranges::find_if(
        m_items,
        [&section, &key](const std::unique_ptr<ConfigItem>& item_ptr) {
            const auto item = item_ptr.get();
            return (item->getSection().compare(section) == 0)
                   and (item->getKey().compare(key) == 0);
        });
    if(result == m_items.end())
    {
        return std::nullopt;
    }

    return static_cast<T&>(**result);
}

inline auto ConfigStorage::begin()
{
    return m_items.begin();
}

inline auto ConfigStorage::end()
{
    return m_items.end();
}

inline auto ConfigStorage::begin() const
{
    return m_items.begin();
}

inline auto ConfigStorage::end() const
{
    return m_items.end();
}

}  // namespace mono::config
