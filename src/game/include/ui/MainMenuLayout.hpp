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
    m_baseMenuW = 512.f;
    m_baseMenuH = 512.f;
    this->BaseLayout::update(workpos, worksize);
    menuY = viewportY + ((viewportH - menuH) / 2.f);
}

void MainMenuLayout::update(const ImVec2& workpos, const ImVec2& worksize)
{
    this->BaseLayout::update(workpos, worksize);
    menuY = viewportY + ((viewportH - menuH) / 2.f);
}

}  // namespace UI
