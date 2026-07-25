#pragma once

#include <algorithm>

#include "BasicConfigItem.hpp"
#include "mono/traits/Arithmetic.hpp"
#include "mono/util/custom_imgui/InputScalar.hpp"

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
    { return std::format("RangeNumberConfigItem<{}>", typeid(T).name()); }

    void drawForDevUi() override
    {
        this->template getValue<T>()
            .and_then([this](T value) -> std::optional<T> {
                if(mono::util::Custom::ImGui::InputScalar<T>(
                       this->getKey().data(),
                       &value,
                       nullptr,
                       nullptr,
                       nullptr,
                       ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    value = std::clamp(value, m_min, m_max);
                    this->setValue(value);
                }

                if(ImGui::IsItemHovered())
                {
                    ImGui::BeginTooltip();
                    ImGui::Text("Range: [%d, %d]", m_min, m_max);
                    ImGui::EndTooltip();
                }
                return value;
            })
            .or_else([this]() -> std::optional<T> {
                T value;
                const std::string label = std::format("{} <missing value>", this->getKey().data());
                ImGui::BeginDisabled();
                mono::util::Custom::ImGui::InputScalar<T>(label, &value);
                ImGui::EndDisabled();
                return std::nullopt;
            });
    }

    const T& getMin() const { return m_min; }

    const T& getMax() const { return m_max; }

    private:
    const T m_min;
    const T m_max;
};

}  // namespace mono::config
