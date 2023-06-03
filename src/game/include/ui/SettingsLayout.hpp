#pragma once

#include "BaseLayout.hpp"

namespace UI
{

struct SettingsLayout : public BaseLayout
{
    inline SettingsLayout(const ImVec2& workpos, const ImVec2& worksize);
    inline void update(const ImVec2& workpos, const ImVec2& worksize) override;
};

SettingsLayout::SettingsLayout(const ImVec2& workpos, const ImVec2& worksize) : BaseLayout(workpos, worksize)
{
    base_menu_w = 1840.f;
    base_menu_h = 660.f;
    this->update(workpos, worksize);
    menu_y = viewport_y + external_h_spacing;
    menu_h = viewport_h - (110.f * scale_h) - (3.f * external_h_spacing);
}

void SettingsLayout::update(const ImVec2& workpos, const ImVec2& worksize)
{
    this->BaseLayout::update(workpos, worksize);
    menu_y = viewport_y + external_h_spacing;
    menu_h = viewport_h - (110.f * scale_h) - (3.f * external_h_spacing);
}

}  // namespace UI
