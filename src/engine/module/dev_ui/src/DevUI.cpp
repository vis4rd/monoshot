#include "dev_ui/DevUI.hpp"

#include <imgui/imgui.h>

#include "config/Config.hpp"

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

        const auto spdlog_level_to_sv = [](spdlog::level level) {
            return spdlog::level_string_views[static_cast<std::underlying_type_t<spdlog::level>>(
                level)];
        };

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
                    if(ImGui::BeginCombo(
                           "LogLevel",
                           spdlog_level_to_sv(mono::config::runtime::logLevel).data()))
                    {
                        for(const auto& level : spdlog::level_string_views)
                        {
                            bool is_selected =
                                (spdlog::level_from_str(std::string{level})
                                 == mono::config::runtime::logLevel);
                            if(ImGui::Selectable(level.data(), is_selected))
                            {
                                mono::config::runtime::logLevel =
                                    spdlog::level_from_str(std::string{level});
                                spdlog::set_level(mono::config::runtime::logLevel);
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
                        using ctWindowMode = mono::config::type::WindowMode;
                        auto& cr_window_mode = mono::config::runtime::windowMode;
                        const auto window_mode_to_string = [](ctWindowMode mode) -> const char* {
                            switch(mode)
                            {
                                case ctWindowMode::WINDOWED: return "Windowed";
                                case ctWindowMode::FULLSCREEN: return "Fullscreen";
                                case ctWindowMode::BORDERLESS: return "Borderless";
                            }
                            return "Unknown";
                        };
                        if(ImGui::BeginCombo("Mode", window_mode_to_string(cr_window_mode)))
                        {
                            for(const auto mode :
                                {ctWindowMode::WINDOWED,
                                 ctWindowMode::FULLSCREEN,
                                 ctWindowMode::BORDERLESS})
                            {
                                const bool is_selected = (mode == cr_window_mode);
                                if(ImGui::Selectable(window_mode_to_string(mode), is_selected))
                                {
                                    cr_window_mode = mode;
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
                        if(ImGui::Checkbox("UseVSync", &mono::config::runtime::useVSync)) { }
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
