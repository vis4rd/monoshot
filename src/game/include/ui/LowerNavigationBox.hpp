#pragma once

#include <ui/external/BeginCombo.hpp>

#include "MainMenuLayout.hpp"

namespace UI
{

// NOLINTNEXTLINE(readability-identifier-naming)
inline void LowerNavigationBox(const UI::BaseLayout& layout)
{
    const ImGuiViewport& main_viewport = *ImGui::GetMainViewport();

    ImGui::SetNextWindowPos({layout.menuX, layout.menuY});
    ImGui::SetNextWindowSize({layout.menuW, layout.menuH});

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {layout.buttonWS, layout.buttonHS});
    ImGui::Begin("LowerNavigationBox", nullptr, layout.windowFlags);
    {
        ImGui::SetCursorScreenPos({layout.menuX + layout.buttonWS, layout.menuY + layout.buttonHS});
        if(ImGui::Button("Close", {layout.buttonW, layout.buttonH}))
        {
            spdlog::debug("Clicking 'Close'");
            auto& sm = SectionManager::get();
            sm.popSection();
        }
        // else if(ImGui::SameLine(); ImGui::Button("Other option", {layout.buttonW,
        // layout.buttonH}))
        // {
        //     spdlog::debug("Clicking 'Other option'");
        // }
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

}  // namespace UI
