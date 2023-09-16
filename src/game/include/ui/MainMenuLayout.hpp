#pragma once

#include "LowerNavigationBoxLayout.hpp"

namespace UI
{

struct MainMenuLayout : public BaseLayout
{
    inline MainMenuLayout(const ImVec2& workpos, const ImVec2& worksize);
    inline void update(const ImVec2& workpos, const ImVec2& worksize) override;
};

MainMenuLayout::MainMenuLayout(const ImVec2& workpos, const ImVec2& worksize)
    : BaseLayout(workpos, worksize)
{
    base_menu_w = 512.f;
    base_menu_h = 512.f;
    this->BaseLayout::update(workpos, worksize);
    menu_y = viewport_y + ((viewport_h - menu_h) / 2.f);
}

void MainMenuLayout::update(const ImVec2& workpos, const ImVec2& worksize)
{
    this->BaseLayout::update(workpos, worksize);
    menu_y = viewport_y + ((viewport_h - menu_h) / 2.f);
}

}  // namespace UI
