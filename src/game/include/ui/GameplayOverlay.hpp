#pragma once

#include <cstdint>

#include <resource/ResourceManager.hpp>

#include "GameplayLayout.hpp"

namespace UI
{

inline void drawOverlay(
    const GameplayLayout& layout,
    const std::int32_t& hero_health,
    const std::int32_t& max_hero_health,
    const std::uint32_t& current_ammo,
    const std::uint32_t& total_ammo,
    const std::uint32_t& weapon_index = 0u)
{
    using res = ResourceManager;
    const auto& font = res::uiAmmoFont;
    auto font_guard = font->use();
    const std::uint32_t weapon_texture_indices[2] = {
        res::rifleInventoryTexture->getID(),
        res::pistolInventoryTexture->getID()};

    ImGui::SetNextWindowPos({layout.menu_x, layout.menu_y});
    ImGui::SetNextWindowSize({layout.menu_w, layout.menu_h});
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {layout.button_w_s, layout.button_h_s});
    ImGui::Begin("Gameplay Overlay", nullptr, layout.window_flags | ImGuiWindowFlags_NoBackground);
    {
        ImGui::Image(
            (void*)(std::intptr_t)weapon_texture_indices[weapon_index],
            {256 * layout.viewport_w / 1920.f, 128 * layout.viewport_h / 1080.f});
        ImGui::Text("%u/%u", current_ammo, total_ammo);
        ImGui::Text("+ %d/%d", hero_health, max_hero_health);
    }
    ImGui::End();
    ImGui::PopStyleVar();
    font_guard.popFont();
}

}  // namespace UI
