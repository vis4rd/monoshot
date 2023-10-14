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
    m_baseMenuW = 1840.f;
    this->update(workpos, worksize);

    spdlog::debug("box size = ({}, {})", menuW, menuH);
    spdlog::debug("box pos = ({}, {})", menuX, menuY);
    spdlog::debug("button size = [{}({}, {}){}]", buttonWS, buttonW, buttonH, buttonHS);
}

void LowerNavigationBoxLayout::update(const ImVec2& workpos, const ImVec2& worksize)
{
    viewportW = worksize.x;
    viewportH = worksize.y;
    viewportX = workpos.x;
    viewportY = workpos.y;

    m_scaleW = viewportW / m_baseViewportW;
    m_scaleH = viewportH / m_baseViewportH;
    externalHSpacing = 40.f * m_scaleH;
    externalWSpacing = 40.f * m_scaleW;

    menuX = viewportX + externalWSpacing;
    menuW = m_baseMenuW * m_scaleW;

    buttonW = m_baseButtonW * m_scaleW;
    buttonH = m_baseButtonH * m_scaleH;
    buttonWS = m_baseWSpacing * m_scaleW;
    buttonHS = m_baseHSpacing * m_scaleH;

    menuH = buttonH + (2.f * buttonHS);
    menuY = viewportY + viewportH - menuH - externalHSpacing;
}

}  // namespace UI
