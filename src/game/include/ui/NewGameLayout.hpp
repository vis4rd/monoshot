#pragma once

#include <ui/BaseLayout.hpp>

namespace UI
{

struct NewGameLayout : public BaseLayout
{
    inline NewGameLayout(const ImVec2& workpos, const ImVec2& worksize);
    inline void update(const ImVec2& workpos, const ImVec2& worksize) override;
};

NewGameLayout::NewGameLayout(const ImVec2& workpos, const ImVec2& worksize)
    : BaseLayout(workpos, worksize)
{
    base_menu_w = 1840.f;
    base_menu_h = 512.f;
    base_button_w = 512.f;
    this->update(workpos, worksize);
    // menu_y = viewport_y + (viewport_h - menu_h) / 2.f;
}

void NewGameLayout::update(const ImVec2& workpos, const ImVec2& worksize)
{
    this->BaseLayout::update(workpos, worksize);
    menu_y = viewport_y + (viewport_h - menu_h) / 2.f;
}

}  // namespace UI
