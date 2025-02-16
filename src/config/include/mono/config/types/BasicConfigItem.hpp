#pragma once

#include <format>
#include <typeinfo>

#include <imgui/imgui.h>
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
        return std::format("BasicConfigItem<{}>", typeid(T).name());
    }

    void drawForDevUi() override
    {
        std::optional<bool> value = this->getValue<bool>();
        if(not value.has_value())
        {
            ImGui::BeginDisabled();
            ImGui::Checkbox(
                std::format("{} <missing value>", this->getKey().data()).c_str(),
                &value.value());
            ImGui::EndDisabled();
            ImGui::SameLine();
            if(ImGui::Button("Set True"))
            {
                this->setValue(true);
            }
            ImGui::SameLine();
            if(ImGui::Button("Set False"))
            {
                this->setValue(false);
            }
            return;
        }
        if(ImGui::Checkbox(this->getKey().data(), &value.value()))
        {
            this->setValue(value.value());
        }
    }
};

}  // namespace mono::config
