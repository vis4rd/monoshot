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
        // auto menu_button_pos = (menu_h - button_h) / 2.f;
        // ImGui::SetCursorPosY(menu_button_pos);
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
