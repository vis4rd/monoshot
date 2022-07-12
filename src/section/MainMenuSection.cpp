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
    ImGui::SetNextWindowPos({main_viewport.WorkPos.x + 10, main_viewport.WorkPos.y + 10});
    ImGui::SetNextWindowSize({400, 600});

    ImGui::Begin("Main Menu", nullptr, window_flags);
    {
        ImGui::Text("Hello :)");
    }
    ImGui::End();
}
