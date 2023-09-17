#pragma once

#include <spdlog/spdlog.h>

#include "SettingsLayout.hpp"

namespace UI
{

struct LowerNavigationBoxLayout : public BaseLayout
{
    inline LowerNavigationBoxLayout(const ImVec2& workpos, const ImVec2& worksize);
    inline void update(const ImVec2& workpos, const ImVec2& worksize) override;
};

LowerNavigationBoxLayout::LowerNavigationBoxLayout(const ImVec2& workpos, const ImVec2& worksize)
    : BaseLayout(workpos, worksize)
{
    base_menu_w = 1840.f;
    this->update(workpos, worksize);

    spdlog::debug("box size = ({}, {})", menu_w, menu_h);
    spdlog::debug("box pos = ({}, {})", menu_x, menu_y);
    spdlog::debug("button size = [{}({}, {}){}]", button_w_s, button_w, button_h, button_h_s);
}

void LowerNavigationBoxLayout::update(const ImVec2& workpos, const ImVec2& worksize)
{
    viewport_w = worksize.x;
    viewport_h = worksize.y;
    viewport_x = workpos.x;
    viewport_y = workpos.y;

    scale_w = viewport_w / base_viewport_w;
    scale_h = viewport_h / base_viewport_h;
    external_h_spacing = 40.f * scale_h;
    external_w_spacing = 40.f * scale_w;

    menu_x = viewport_x + external_w_spacing;
    menu_w = base_menu_w * scale_w;

    button_w = base_button_w * scale_w;
    button_h = base_button_h * scale_h;
    button_w_s = base_w_spacing * scale_w;
    button_h_s = base_h_spacing * scale_h;

    menu_h = button_h + (2.f * button_h_s);
    menu_y = viewport_y + viewport_h - menu_h - external_h_spacing;
}

}  // namespace UI
