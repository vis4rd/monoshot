#include "dev_ui/DevUI.hpp"

#include <imgui/imgui.h>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>

#include "mono/config/Config.hpp"

namespace mono::dev_ui
{

namespace state
{
// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)

static bool settings_window = false;
// static bool demo_window = false;

// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
}  // namespace state

void initialize() { }

void render()
{
    if constexpr(mono::config::constant::debugBuild)
    {
        constexpr ImGuiWindowFlags window_flags =
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings;

        float previous_pos_y{};
        float previous_size_y{};
        const auto update_prev_window = [&previous_pos_y, &previous_size_y]() {
            previous_pos_y = ImGui::GetWindowPos().y;
            previous_size_y = ImGui::GetWindowSize().y;
        };
        const auto y_after_previous = [&previous_pos_y, &previous_size_y](float offset) {
            return previous_pos_y + previous_size_y + offset;
        };
        const float right_window_edge = ImGui::GetIO().DisplaySize.x - 10.0f;
        const ImVec2 right_align_pivot = {1.0f, 0.0f};

        ImGui::SetNextWindowPos(
            ImVec2(right_window_edge, 10.0f),
            ImGuiCond_Always,
            ImVec2(1.0f, 0.0f));
        ImGui::Begin("Dev UI", nullptr, window_flags);
        {
            update_prev_window();
            ImGui::Selectable("Settings", &state::settings_window);
            // ImGui::Selectable("Demo", &state::demo_window);
        }
        ImGui::End();

        if(state::settings_window)
        {
            ImGui::SetNextWindowPos(
                {right_window_edge, y_after_previous(10.0f)},
                ImGuiCond_Always,
                right_align_pivot);
            ImGui::Begin("Settings", &state::settings_window, window_flags);
            {
                update_prev_window();
                ImGui::SeparatorText("Engine");

                {
                    auto loglevel_config =
                        mono::config::runtime
                            .get<config::OptionStringConfigItem>("engine", "LogLevel")
                            .value()
                            .get();
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

                ImGui::SeparatorText("Window");
                {
                    {
                        auto window_mode_config =
                            mono::config::runtime
                                .get<config::OptionStringConfigItem>("engine.window", "Mode")
                                .value()
                                .get();
                        const auto current_window_mode =
                            window_mode_config.getValue<std::string>().value();
                        if(ImGui::BeginCombo("Mode", current_window_mode.data()))
                        {
                            ;
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
                            mono::config::runtime
                                .get<config::BasicConfigItem<bool>>("engine.window", "UseVSync")
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
            ImGui::End();
        }

        // if(state::demo_window)
        // {
        //     ImGui::SetNextWindowPos(
        //         {right_window_edge, y_after_previous(10.0f)},
        //         ImGuiCond_Always,
        //         right_align_pivot);
        //     ImGui::Begin("Test123", &state::demo_window, window_flags);
        //     {
        //         update_prev_window();
        //         ImGui::Text("Hello, world!");
        //     }
        //     ImGui::End();
        // }
    }
}

}  // namespace mono::dev_ui
