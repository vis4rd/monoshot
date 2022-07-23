#pragma once

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
    base_menu_h = 110.f;
}

void LowerNavigationBoxLayout::update(const ImVec2& workpos, const ImVec2& worksize)
{
    this->BaseLayout::update(workpos, worksize);
}

}  // namespace UI
