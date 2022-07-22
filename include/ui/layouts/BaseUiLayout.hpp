#pragma once

#include "../../window/Window.hpp"

namespace UI
{

struct BaseUiLayout
{
    BaseUiLayout(const ImVec2& workpos, const ImVec2& worksize, const ImVec2& menusize)
        : base_menu_w(menusize.x), base_menu_h(menusize.y), viewport_w(worksize.x), viewport_h(worksize.y),
          viewport_x(workpos.x), viewport_y(workpos.y)
    {
    }

    const float base_viewport_w = 1920.f;
    const float base_viewport_h = 1080.f;
    const float base_menu_w;
    const float base_menu_h;
    const float base_button_w = 256.f;
    const float base_button_h = 64.f;
    const float base_w_spacing = 23.f;
    const float base_h_spacing = 8.f;
    const float base_external_w_spacing = 40.f;
    const float base_external_h_spacing = 40.f;

    const float viewport_w;
    const float viewport_h;
    const float viewport_x;
    const float viewport_y;

    const float scale_w = viewport_w / base_viewport_w;
    const float scale_h = viewport_h / base_viewport_h;

    float external_h_spacing = 40.f * scale_h;
    float external_w_spacing = 40.f * scale_w;
    float menu_w = base_menu_w * scale_w;
    float menu_h = base_menu_h * scale_h;
    float menu_x = viewport_x + external_w_spacing;
    float menu_y = viewport_y + viewport_h - menu_h - external_h_spacing;
    float button_w = base_button_w * scale_w;
    float button_h = base_button_h * scale_h;
    float button_w_s = base_w_spacing * scale_w;
    float button_h_s = base_h_spacing * scale_h;


    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove
                                    | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse
                                    | ImGuiWindowFlags_NoNav
                                    // | ImGuiWindowFlags_NoBackground  // uncomment when everything setup
                                    | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoDocking;
};

}  // namespace UI
