#pragma once

#include <concepts>
#include <cstddef>
#include <exception>
#include <format>
#include <ranges>
#include <typeinfo>

#include <inicpp.h>
#include <spdlog/spdlog.h>

#include "../ConfigItem.hpp"

namespace mono::config
{

namespace priv
{
template<typename T>
concept ArithmeticTrait = std::integral<T> or std::floating_point<T>;
}

template<std::size_t NUM, priv::ArithmeticTrait T, char SEP = ','>
class MultiNumberConfigItem : public ConfigItem
{
    public:
    MultiNumberConfigItem(
        ini::IniFile& ini_storage,
        const std::string& section,
        const std::string& key)
        : ConfigItem(ini_storage, section, key)
    { }

    bool isValid() const override
    {
        auto split_view = this->getNativeValue() | std::ranges::views::split(SEP)
                          | std::ranges::views::transform([](auto&& subrange) {
                                return std::string{subrange.begin(), subrange.end()};
                            });

        if(std::ranges::distance(split_view) != NUM)
        {
            return false;
        }

        ini::Convert<T> converter{};
        T temp{};
        try
        {
            for(const auto& dim : split_view)
            {
                converter.decode(dim, temp);
            }
        }
        catch(const std::exception& err)
        {
            return false;
        }
        return true;
    }

    std::string getType() const override
    {
        return std::format("MultiNumberConfigItem<{},{},'{}'>", NUM, typeid(T).name(), SEP);
    };
};

}  // namespace mono::config
