#include "dev_ui/DevUI.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>

#include "dev_ui/priv/StateData.hpp"
#include "mono/config/Config.hpp"

namespace mono::dev_ui
{

namespace data
{

// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
float previous_pos_y{};
float previous_size_y{};
constexpr ImGuiWindowFlags window_flags =
    ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings;
const float right_window_edge = ImGui::GetIO().DisplaySize.x - 10.0f;
const ImVec2 right_align_pivot = {1.0f, 0.0f};
const ImVec2 dev_ui_menu_size = []() {
    const auto& io = ImGui::GetIO();
    return ImVec2{
        150.f,
        50.f
            + static_cast<float>(priv::state::window_visibility_flags.size()) * io.FontGlobalScale
                  * (ImGui::GetCurrentContext()->FontSize
                     + ImGui::GetCurrentContext()->Style.ItemSpacing.y)};
}();
// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

}  // namespace data

static void updatePrevWindow()
{
    data::previous_pos_y = ImGui::GetWindowPos().y;
    data::previous_size_y = ImGui::GetWindowSize().y;
}

static float nextWindowPosY(float offset)
{
    return data::previous_pos_y + data::previous_size_y + offset;
}

void initialize()
{
    priv::state::window_visibility_flags["Settings"] = false;
}

void render()
{
    if constexpr(mono::config::constant::debugBuild)
    {
        ImGui::SetNextWindowPos(
            ImVec2(data::right_window_edge, nextWindowPosY(10.0f)),
            ImGuiCond_Always,
            data::right_align_pivot);
        ImGui::SetNextWindowSize(data::dev_ui_menu_size);
        ImGui::Begin("Dev UI", nullptr, data::window_flags);
        {
            updatePrevWindow();
            for(auto& [window_name, flag] : priv::state::window_visibility_flags)
            {
                ImGui::Selectable(window_name.c_str(), &flag);
            }
        }
        ImGui::End();

        if(priv::state::window_visibility_flags["Settings"])
        {
            ImGui::SetNextWindowPos(
                {data::right_window_edge, nextWindowPosY(10.0f)},
                ImGuiCond_Always,
                data::right_align_pivot);
            ImGui::Begin(
                "Settings",
                &priv::state::window_visibility_flags["Settings"],
                data::window_flags);
            {
                updatePrevWindow();
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
    }
}

}  // namespace mono::dev_ui
