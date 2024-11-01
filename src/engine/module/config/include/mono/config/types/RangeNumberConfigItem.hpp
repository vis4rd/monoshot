#pragma once

#include "BasicConfigItem.hpp"
#include "traits/Arithmetic.hpp"

namespace mono::config
{

template<ArithmeticTrait T>
class RangeNumberConfigItem : public BasicConfigItem<T>
{
    public:
    RangeNumberConfigItem(
        ini::IniFile& ini_storage,
        const std::string& section,
        const std::string& key,
        const T& min,
        const T& max)
        : BasicConfigItem<T>(ini_storage, section, key)
        , m_min(min)
        , m_max(max)
    { }

    bool isValid() const override
    {
        if(not BasicConfigItem<T>::isValid())
        {
            return false;
        }

        const T value = this->template getValue<T>().value();
        if(value < m_min or value > m_max)
        {
            spdlog::error(
                "Config field [{}][{}] has value {} which is out of range [{}, {}]",
                this->getSection(),
                this->getKey(),
                value,
                m_min,
                m_max);
            return false;
        }

        return true;
    }

    std::string getType() const override
    {
        return std::format("RangeNumberConfigItem<{}>", typeid(T).name());
    }

    const T& getMin() const { return m_min; }

    const T& getMax() const { return m_max; }

    private:
    const T m_min;
    const T m_max;
};

}  // namespace mono::config
