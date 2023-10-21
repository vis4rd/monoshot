#pragma once

#include <imgui/imgui.h>

#include "config/StaticConfiguration.hpp"

// NOLINTNEXTLINE(readability-identifier-naming)
namespace UI
{

struct BaseLayout
{
    inline BaseLayout(const ImVec2& workpos, const ImVec2& worksize);
    inline BaseLayout(const BaseLayout& copy) = default;
    inline BaseLayout(BaseLayout&& move) noexcept = default;
    virtual inline ~BaseLayout() = default;

    inline BaseLayout& operator=(const BaseLayout& copy) = default;
    inline BaseLayout& operator=(BaseLayout&& move) noexcept = default;
    virtual inline void update(const ImVec2& workpos, const ImVec2& worksize);

    protected:
    float m_baseViewportW = 1920.f;
    float m_baseViewportH = 1080.f;
    float m_baseMenuW = 1920.f;
    float m_baseMenuH = 1080.f;
    float m_baseButtonW = 256.f;
    float m_baseButtonH = 64.f;
    float m_baseWSpacing = 23.f;
    float m_baseHSpacing = 23.f;
    float m_baseExternalWSpacing = 40.f;
    float m_baseExternalHSpacing = 40.f;

    public:
    float viewportW;
    float viewportH;
    float viewportX;
    float viewportY;

    protected:
    float m_scaleW = viewportW / m_baseViewportW;
    float m_scaleH = viewportH / m_baseViewportH;

    public:
    float externalHSpacing = 40.f * m_scaleH;
    float externalWSpacing = 40.f * m_scaleW;
    float menuW = m_baseMenuW * m_scaleW;
    float menuH = m_baseMenuH * m_scaleH;
    float menuX = viewportX + externalWSpacing;
    float menuY = viewportY + viewportH - menuH - externalHSpacing;
    float buttonW = m_baseButtonW * m_scaleW;
    float buttonH = m_baseButtonH * m_scaleH;
    float buttonWS = m_baseWSpacing * m_scaleW;
    float buttonHS = m_baseHSpacing * m_scaleH;


    ImGuiWindowFlags windowFlags =
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNav
        | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoDocking;
};

BaseLayout::BaseLayout(const ImVec2& workpos, const ImVec2& worksize)
    : viewportW(worksize.x)
    , viewportH(worksize.y)
    , viewportX(workpos.x)
    , viewportY(workpos.y)
{
    if constexpr(not mono::config::constant::debugMode)
    {
        windowFlags |= ImGuiWindowFlags_NoBackground;
    }
}

void BaseLayout::update(const ImVec2& workpos, const ImVec2& worksize)
{
    viewportW = worksize.x;
    viewportH = worksize.y;
    viewportX = workpos.x;
    viewportY = workpos.y;

    m_scaleW = viewportW / m_baseViewportW;
    m_scaleH = viewportH / m_baseViewportH;

    externalHSpacing = 40.f * m_scaleH;
    externalWSpacing = 40.f * m_scaleW;
    menuW = m_baseMenuW * m_scaleW;
    menuH = m_baseMenuH * m_scaleH;
    menuX = viewportX + externalWSpacing;
    menuY = viewportY + viewportH - menuH - externalHSpacing;
    buttonW = m_baseButtonW * m_scaleW;
    buttonH = m_baseButtonH * m_scaleH;
    buttonWS = m_baseWSpacing * m_scaleW;
    buttonHS = m_baseHSpacing * m_scaleH;
}

}  // namespace UI
