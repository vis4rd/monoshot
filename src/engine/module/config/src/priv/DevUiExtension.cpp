#include "config/priv/DevUiExtension.hpp"

#include <imgui/imgui.h>

#include "config/Config.hpp"

namespace mono::config::priv
{

void devUiExtension()
{
    ImGui::SeparatorText("Engine");
    {
        auto loglevel_config =
            runtime.get<config::OptionStringConfigItem>("engine", "LogLevel").value().get();
        const auto current_loglevel = loglevel_config.getValue<std::string>().value();
        if(ImGui::BeginCombo("LogLevel", current_loglevel.data()))
        {
            for(const auto& level : spdlog::level_string_views)
            {
                bool is_selected = (current_loglevel.compare(level) == 0);
                if(ImGui::Selectable(level.data(), is_selected))
                {
                    bool success = loglevel_config.setValue(std::string{level});
                    if(success)
                    {
                        spdlog::set_level(spdlog::level_from_str(std::string{level}));
                    }
                }
                if(is_selected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
        ImGui::SameLine();
        ImGui::Dummy({150.0f, 0.0f});
    }

    ImGui::SeparatorText("Engine.Window");
    {
        {
            auto window_mode_config =
                runtime.get<config::OptionStringConfigItem>("engine.window", "Mode").value().get();
            const auto current_window_mode = window_mode_config.getValue<std::string>().value();
            if(ImGui::BeginCombo("Mode", current_window_mode.data()))
            {
                for(const auto& mode : window_mode_config.getOptions())
                {
                    const bool is_selected = (current_window_mode.compare(mode) == 0);
                    if(ImGui::Selectable(mode.c_str(), is_selected))
                    {
                        window_mode_config.setValue(mode);
                    }
                    if(is_selected)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
        }
        {
            auto vsync_config =
                runtime.get<config::BasicConfigItem<bool>>("engine.window", "UseVSync")
                    .value()
                    .get();
            bool current_vsync = vsync_config.getValue<bool>().value();
            if(ImGui::Checkbox("UseVSync", &current_vsync))
            {
                vsync_config.setValue(current_vsync);
            }
        }
    }
}

}  // namespace mono::config::priv
