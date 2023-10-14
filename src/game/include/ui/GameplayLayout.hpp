#pragma once

#include <ui/BaseLayout.hpp>

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
    m_baseMenuW = 384.f;
    m_baseMenuH = 512.f;
    this->BaseLayout::update(workpos, worksize);
    menuY = viewportY + viewportH - menuH;
}

void GameplayLayout::update(const ImVec2& workpos, const ImVec2& worksize)
{
    this->BaseLayout::update(workpos, worksize);
    menuY = viewportY + viewportH - menuH;
}

}  // namespace UI
