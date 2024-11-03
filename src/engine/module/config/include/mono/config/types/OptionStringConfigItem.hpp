#pragma once

#include <spdlog/spdlog.h>

#include "../ConfigItem.hpp"

namespace mono::config
{

class OptionStringConfigItem final : public ConfigItem
{
    public:
    OptionStringConfigItem(
        ini::IniFile& ini_storage,
        const std::string& section,
        const std::string& key,
        const std::vector<std::string>& options)
        : ConfigItem(ini_storage, section, key)
        , m_options(options)
    { }

    bool isValid() const override
    {
        const std::string value = this->getNativeValue();
        return std::ranges::any_of(m_options, [&value](const std::string& option) {
            return value.compare(option) == 0;
        });
    }

    constexpr std::string_view getType() const override { return "OptionStringConfigItem"; };

    const std::vector<std::string>& getOptions() const { return m_options; }

    private:
    const std::vector<std::string> m_options{};
};

}  // namespace mono::config
