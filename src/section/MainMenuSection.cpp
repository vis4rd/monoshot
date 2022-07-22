#include "../../include/section/MainMenuSection.hpp"

MainMenuSection::MainMenuSection() : Section()
{
    m_name = "MainMenuSection";
    auto& input_manager = Input::get();
    auto group_id = input_manager.addGroup(m_name);
    input_manager.addKeybind(group_id,
        GLFW_KEY_ESCAPE,
        GLFW_PRESS,
        []
        {
            SectionManager::get().clear();
        });
}

void MainMenuSection::update() noexcept { }

void MainMenuSection::render() noexcept
{
    const ImGuiViewport& main_viewport = *ImGui::GetMainViewport();
    auto main_window_layout = UI::BaseUiLayout(main_viewport.WorkPos, main_viewport.WorkSize, {512.f, 512.f});
    main_window_layout.menu_y =
        main_window_layout.viewport_y + ((main_window_layout.viewport_h - main_window_layout.menu_h) / 2.f);

    ImGui::SetNextWindowPos({main_window_layout.menu_x, main_window_layout.menu_y - 100.f});
    ImGui::Begin("MainMenuGameTitle", nullptr, main_window_layout.window_flags);
    {
        ImGui::Text("Top-down Shooter Game");
    }
    ImGui::End();

    ImGui::SetNextWindowPos({main_window_layout.menu_x, main_window_layout.menu_y});
    ImGui::SetNextWindowSize({main_window_layout.menu_w, main_window_layout.menu_h});
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {main_window_layout.button_w_s, main_window_layout.button_h_s});
    ImGui::Begin("MainMenu", nullptr, main_window_layout.window_flags);
    {
        if(ImGui::Button("New game", {main_window_layout.button_w, main_window_layout.button_h}))
        {
            spdlog::debug("Clicking 'New game'");
            auto& sm = SectionManager::get();
            sm.emplaceSection<DebugSection>();
        }
        else if(ImGui::Button("Settings", {main_window_layout.button_w, main_window_layout.button_h}))
        {
            spdlog::debug("Clicking 'Settings'");
            auto& sm = SectionManager::get();
            sm.emplaceSection<SettingsSection>();
        }
        else if(ImGui::Button("Exit", {main_window_layout.button_w, main_window_layout.button_h}))
        {
            spdlog::debug("Clicking 'Exit'");
            SectionManager::get().clear();
        }
    }
    ImGui::End();
    ImGui::PopStyleVar();
}
