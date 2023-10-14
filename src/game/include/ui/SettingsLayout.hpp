#pragma once

#include <ui/BaseLayout.hpp>

namespace UI
{

struct SettingsLayout : public BaseLayout
{
    inline SettingsLayout(const ImVec2& workpos, const ImVec2& worksize);
    inline void update(const ImVec2& workpos, const ImVec2& worksize) override;
};

SettingsLayout::SettingsLayout(const ImVec2& workpos, const ImVec2& worksize)
    : BaseLayout(workpos, worksize)
{
    m_baseMenuW = 1840.f;
    m_baseMenuH = 660.f;
    this->update(workpos, worksize);
    menuY = viewportY + externalHSpacing;
    menuH = viewportH - (110.f * m_scaleH) - (3.f * externalHSpacing);
}

void SettingsLayout::update(const ImVec2& workpos, const ImVec2& worksize)
{
    this->BaseLayout::update(workpos, worksize);
    menuY = viewportY + externalHSpacing;
    menuH = viewportH - (110.f * m_scaleH) - (3.f * externalHSpacing);
}

}  // namespace UI
