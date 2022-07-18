#pragma once

#include "../../window/Window.hpp"
#include "external/BeginCombo.hpp"

namespace UI
{

inline void LowerNavigationBox()
{
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoScrollbar;
    window_flags |= ImGuiWindowFlags_NoScrollWithMouse;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoNav;
    // window_flags |= ImGuiWindowFlags_NoBackground;  // uncomment when everything setup
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
    window_flags |= ImGuiWindowFlags_NoDocking;

    const ImGuiViewport& main_viewport = *ImGui::GetMainViewport();
    auto& style = ImGui::GetStyle();

    auto base_viewport_w = 1920.f;
    auto base_viewport_h = 1080.f;
    auto base_menu_w = 1840.f;
    auto base_menu_h = 110.f;
    auto base_button_w = 256.f;
    auto base_button_h = 64.f;
    auto base_h_spacing = 8.f;
    auto base_w_spacing = 23.f;
    auto base_external_w_spacing = 40.f;
    auto base_external_h_spacing = 40.f;


    auto& [viewport_w, viewport_h] = main_viewport.WorkSize;
    auto& [viewport_x, viewport_y] = main_viewport.WorkPos;

    auto scale_w = viewport_w / base_viewport_w;
    auto scale_h = viewport_h / base_viewport_h;

    auto external_h_spacing = 40.f * scale_h;
    auto external_w_spacing = 40.f * scale_w;
    auto menu_w = base_menu_w * scale_w;
    auto menu_h = base_menu_h * scale_h;
    auto menu_x = viewport_x + external_w_spacing;
    auto menu_y = viewport_y + viewport_h - menu_h - external_h_spacing;
    auto button_w = base_button_w * scale_w;
    auto button_h = base_button_h * scale_h;
    auto button_w_s = base_w_spacing * scale_w;
    auto button_h_s = base_h_spacing * scale_h;

    ImGui::SetNextWindowPos({menu_x, menu_y});
    ImGui::SetNextWindowSize({menu_w, menu_h});

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {button_w_s, button_h_s});
    ImGui::Begin("LowerNavigationBox", nullptr, window_flags);
    {
        // auto menu_button_pos = (menu_h - button_h) / 2.f;
        // ImGui::SetCursorPosY(menu_button_pos);
        if(ImGui::Button("Close", {button_w, button_h}))
        {
            spdlog::debug("Clicking 'Close'");
            auto& sm = SectionManager::get();
            sm.popSection();
        }
        else if(ImGui::SameLine(); ImGui::Button("Other option", {button_w, button_h}))
        {
            spdlog::debug("Clicking 'Other option'");
        }
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

}  // namespace UI
