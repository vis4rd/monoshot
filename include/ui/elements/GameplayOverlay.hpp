#pragma once

#include "../layouts/GameplayLayout.hpp"
#include "../../utility/ResourceManager.hpp"

#include <cstdint>

namespace UI
{

struct GameplayLayout;

inline void drawOverlay(const GameplayLayout& layout, const std::int32_t& hero_health, const std::int32_t& max_hero_health, const std::uint32_t& current_ammo, const std::uint32_t& total_ammo, const std::uint32_t& weapon_index = 0u)
{
    using res = ResourceManager;
    const auto& font = res::uiAmmoFont;
    auto font_guard = font->use();

    ImGui::SetNextWindowPos({layout.menu_x, layout.menu_y});
    ImGui::SetNextWindowSize({layout.menu_w, layout.menu_h});
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {layout.button_w_s, layout.button_h_s});
    ImGui::Begin("Gameplay Overlay", nullptr, layout.window_flags | ImGuiWindowFlags_NoBackground);
    {
        ImGui::Text("(%u): %u/%u", weapon_index + 1u, current_ammo, total_ammo);
        ImGui::Text("+ %d/%d", hero_health, max_hero_health);
    }
    ImGui::End();
    ImGui::PopStyleVar();
    font_guard.popFont();
}

}  // namespace UI
