#pragma once

#include <format>
#include <typeinfo>

#include <spdlog/spdlog.h>

#include "../ConfigItem.hpp"
#include "../priv/IniCompliantTrait.hpp"

namespace mono::config
{

template<IniCompliantTrait T>
class BasicConfigItem : public ConfigItem
{
    public:
    BasicConfigItem(ini::IniFile& ini_storage, const std::string& section, const std::string& key)
        : ConfigItem(ini_storage, section, key)
    { }

    bool isValid() const override
    {
        ini::Convert<T> converter{};
        T temp{};
        try
        {
            converter.decode(this->getNativeValue(), temp);
        }
        catch(const std::exception& e)
        {
            spdlog::error(
                "Failed to decode BasicConfigItem[{}][{}]: {}",
                this->getSection(),
                this->getKey(),
                e.what());
            return false;
        }
        return true;
    }

    std::string getType() const override
    {
        return std::format("BasicConfigType<{}>", typeid(T).name());
    }
};

}  // namespace mono::config
