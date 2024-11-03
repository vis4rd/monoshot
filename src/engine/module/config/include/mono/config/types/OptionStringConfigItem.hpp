#pragma once

#include <iterator>
#include <numeric>

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
        const bool result = std::ranges::any_of(m_options, [&value](const std::string& option) {
            return value.compare(option) == 0;
        });
        if(not result)
        {
            const std::string options_list = std::accumulate(
                std::next(m_options.begin()),
                m_options.end(),
                m_options[0],
                [](const std::string& a, const std::string& b) {
                    return a + ", " + b;
                });
            spdlog::error(
                "Config field [{}][{}] has value '{}' which is not among valid options: [{}]",
                this->getSection(),
                this->getKey(),
                value,
                options_list);
        }
        return result;
    }

    std::string getType() const override { return "OptionStringConfigItem"; };

    const std::vector<std::string>& getOptions() const { return m_options; }

    private:
    const std::vector<std::string> m_options{};
};

}  // namespace mono::config
