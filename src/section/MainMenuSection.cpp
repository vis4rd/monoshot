#include "../../include/section/MainMenuSection.hpp"

MainMenuSection::MainMenuSection() : Section() { }

void MainMenuSection::update() noexcept { }

void MainMenuSection::render() noexcept
{
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoScrollbar;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoNav;
    // window_flags |= ImGuiWindowFlags_NoBackground;  // uncomment when everything setup
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
    window_flags |= ImGuiWindowFlags_NoDocking;

    const ImGuiViewport& main_viewport = *ImGui::GetMainViewport();
    auto& style = ImGui::GetStyle();

    auto base_viewport_w = 1920.f;
    auto base_viewport_h = 1080.f;
    auto base_menu_w = 512.f;
    auto base_menu_h = 512.f;
    auto base_button_w = 256.f;
    auto base_button_h = 64.f;
    auto base_h_spacing = 8.f;

    auto& [viewport_w, viewport_h] = main_viewport.WorkSize;
    auto& [viewport_x, viewport_y] = main_viewport.WorkPos;
    auto scale_w = viewport_w / base_viewport_w;
    auto scale_h = viewport_h / base_viewport_h;

    auto menu_w = base_menu_w * scale_w;
    auto menu_h = base_menu_h * scale_h;
    auto menu_x = viewport_x + 40.f;
    auto menu_y = viewport_y + ((viewport_h - menu_h) / 2.f);
    auto button_w = base_button_w * scale_w;
    auto button_h = base_button_h * scale_h;
    auto button_w_s = base_h_spacing * scale_w;
    auto button_h_s = base_h_spacing * scale_h;

    ImGui::Begin("Main Menu", nullptr, window_flags);
    {
        ImGui::Text("Top-down Shooter Game");
    }
    ImGui::End();


    ImGui::SetNextWindowPos({menu_x, menu_y});
    ImGui::SetNextWindowSize({menu_w, menu_h});
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {button_w_s, button_h_s});
    ImGui::Begin("Main Menu", nullptr, window_flags);
    {
        if(ImGui::Button("New game", {button_w, button_h}))
        {
            auto& sm = SectionManager::get();
            sm.emplaceSection<DebugSection>();
            ImGui::End();
            ImGui::PopStyleVar();
            return;
        }
        if(ImGui::Button("Settings", {button_w, button_h})) { }
        if(ImGui::Button("Exit", {button_w, button_h})) { }
    }
    ImGui::End();
    ImGui::PopStyleVar();
}
