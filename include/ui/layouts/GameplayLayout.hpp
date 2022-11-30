#pragma once

#include "BaseLayout.hpp"

namespace UI
{

struct GameplayLayout : public BaseLayout
{
    inline GameplayLayout(const ImVec2& workpos, const ImVec2& worksize);
    inline void update(const ImVec2& workpos, const ImVec2& worksize) override;
};

GameplayLayout::GameplayLayout(const ImVec2& workpos, const ImVec2& worksize)
    : BaseLayout(workpos, worksize)
{
    base_menu_w = 384.f;
    base_menu_h = 512.f;
    this->BaseLayout::update(workpos, worksize);
    menu_y = viewport_y + viewport_h - menu_h;
}

void GameplayLayout::update(const ImVec2& workpos, const ImVec2& worksize)
{
    this->BaseLayout::update(workpos, worksize);
    menu_y = viewport_y + viewport_h - menu_h;
}

}  // namespace UI
