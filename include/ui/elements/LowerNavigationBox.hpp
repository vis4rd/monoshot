#pragma once

#include "external/BeginCombo.hpp"
#include "../layouts/MainMenuLayout.hpp"

namespace UI
{

inline void LowerNavigationBox(const UI::BaseLayout& layout)
{
    const ImGuiViewport& main_viewport = *ImGui::GetMainViewport();

    ImGui::SetNextWindowPos({layout.menu_x, layout.menu_y});
    ImGui::SetNextWindowSize({layout.menu_w, layout.menu_h});

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {layout.button_w_s, layout.button_h_s});
    ImGui::Begin("LowerNavigationBox", nullptr, layout.window_flags);
    {
        ImGui::SetCursorScreenPos({layout.menu_x + layout.button_w_s, layout.menu_y + layout.button_h_s});
        if(ImGui::Button("Close", {layout.button_w, layout.button_h}))
        {
            spdlog::debug("Clicking 'Close'");
            auto& sm = SectionManager::get();
            sm.popSection();
        }
        else if(ImGui::SameLine(); ImGui::Button("Other option", {layout.button_w, layout.button_h}))
        {
            spdlog::debug("Clicking 'Other option'");
        }
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

}  // namespace UI
