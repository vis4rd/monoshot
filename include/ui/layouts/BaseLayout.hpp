#pragma once

#include "../../window/Window.hpp"

namespace UI
{

struct BaseLayout
{
    inline BaseLayout(const ImVec2& workpos, const ImVec2& worksize);
    virtual inline void update(const ImVec2& workpos, const ImVec2& worksize);

    protected:
    const float base_viewport_w = 1920.f;
    const float base_viewport_h = 1080.f;
    float base_menu_w = 1920.f;
    float base_menu_h = 1080.f;
    const float base_button_w = 256.f;
    const float base_button_h = 64.f;
    const float base_w_spacing = 23.f;
    const float base_h_spacing = 8.f;
    const float base_external_w_spacing = 40.f;
    const float base_external_h_spacing = 40.f;

    public:
    float viewport_w;
    float viewport_h;
    float viewport_x;
    float viewport_y;

    protected:
    float scale_w = viewport_w / base_viewport_w;
    float scale_h = viewport_h / base_viewport_h;

    public:
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

BaseLayout::BaseLayout(const ImVec2& workpos, const ImVec2& worksize)
    : viewport_w(worksize.x), viewport_h(worksize.y), viewport_x(workpos.x), viewport_y(workpos.y)
{
}

void BaseLayout::update(const ImVec2& workpos, const ImVec2& worksize)
{
    viewport_w = worksize.x;
    viewport_h = worksize.y;
    viewport_x = workpos.x;
    viewport_y = workpos.y;

    scale_w = viewport_w / base_viewport_w;
    scale_h = viewport_h / base_viewport_h;

    external_h_spacing = 40.f * scale_h;
    external_w_spacing = 40.f * scale_w;
    menu_w = base_menu_w * scale_w;
    menu_h = base_menu_h * scale_h;
    menu_x = viewport_x + external_w_spacing;
    menu_y = viewport_y + viewport_h - menu_h - external_h_spacing;
    button_w = base_button_w * scale_w;
    button_h = base_button_h * scale_h;
    button_w_s = base_w_spacing * scale_w;
    button_h_s = base_h_spacing * scale_h;
}

}  // namespace UI
