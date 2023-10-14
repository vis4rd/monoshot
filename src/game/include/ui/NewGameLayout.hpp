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
    m_baseMenuW = 1840.f;
    m_baseMenuH = 512.f;
    m_baseButtonW = 512.f;
    this->update(workpos, worksize);
    // menuY = viewportY + (viewportH - menuH) / 2.f;
}

void NewGameLayout::update(const ImVec2& workpos, const ImVec2& worksize)
{
    this->BaseLayout::update(workpos, worksize);
    menuY = viewportY + (viewportH - menuH) / 2.f;
}

}  // namespace UI
