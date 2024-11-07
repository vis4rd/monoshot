#include "dev_ui/DevUI.hpp"

#include <spdlog/common.h>
#include <spdlog/spdlog.h>

#include "mono/config/Config.hpp"

namespace mono::dev_ui
{

namespace data
{
// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)

float right_window_edge;
const ImVec2 right_align_pivot = {1.0f, 0.0f};
ImVec2 dev_ui_menu_size;

// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
}  // namespace data

static float nextWindowPosY(float offset)
{
    return data::previous_pos_y + data::previous_size_y + offset;
}

static ImVec2 getDevUiMenuSize()
{
    const auto& io = ImGui::GetIO();
    return ImVec2{
        150.f,
        50.f
            + static_cast<float>(priv::state::window_visibility_flags.size()) * io.FontGlobalScale
                  * (ImGui::GetCurrentContext()->FontSize
                     + ImGui::GetCurrentContext()->Style.ItemSpacing.y)};
}

static void renderDevUiMenu()
{
    ImGui::SetNextWindowPos(
        ImVec2(data::right_window_edge, nextWindowPosY(10.0f)),
        ImGuiCond_Always,
        data::right_align_pivot);
    ImGui::SetNextWindowSize(data::dev_ui_menu_size);
    ImGui::Begin("Dev UI", nullptr, data::window_flags);
    {
        priv::updatePrevWindow();
        for(auto& [window_name, flag] : priv::state::window_visibility_flags)
        {
            ImGui::Selectable(window_name.c_str(), &flag);
        }
    }
    ImGui::End();
}

static void prepareRegisteredWindowForDraw(const std::string& window_name)
{
    if(not priv::state::window_visibility_flags.contains(window_name))
    {
        return;
    }
    if(priv::state::window_visibility_flags[window_name])
    {
        ImGui::SetNextWindowPos(
            {data::right_window_edge, nextWindowPosY(10.0f)},
            ImGuiCond_Always,
            data::right_align_pivot);
        ImGui::Begin(
            window_name.c_str(),
            &priv::state::window_visibility_flags[window_name],
            data::window_flags);
        {
            priv::updatePrevWindow();
        }
        ImGui::End();
    }
}

static void updateData()
{
    data::previous_pos_y = 0.f;
    data::previous_size_y = 0.f;
    data::right_window_edge = ImGui::GetIO().DisplaySize.x - 10.0f;
    data::dev_ui_menu_size = getDevUiMenuSize();
}

void initialize()
{
    if constexpr(mono::config::constant::debugBuild)
    {
        // prepare data for the first frame
        updateData();

        // register default windows
        registerWindow("Test123");
    }
}

void render()
{
    if constexpr(mono::config::constant::debugBuild)
    {
        updateData();

        renderDevUiMenu();

        for(const auto& [window_name, flag] : priv::state::window_visibility_flags)
        {
            prepareRegisteredWindowForDraw(window_name);
        }

        drawToWindow("Test123", []() {
            ImGui::Text("Test123");
        });
    }
}

void registerWindow(const std::string& name)
{
    priv::state::window_visibility_flags[name] = false;
}

}  // namespace mono::dev_ui
