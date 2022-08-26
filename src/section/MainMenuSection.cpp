#include "../../include/section/MainMenuSection.hpp"

#include "../../include/section/CreatorSection.hpp"

MainMenuSection::MainMenuSection()
    : Section(),
      m_layout(ImGui::GetMainViewport()->WorkPos, ImGui::GetMainViewport()->WorkSize)
{
    m_name = "MainMenuSection";
    auto& input_manager = InputManager::get();
    auto group_id = input_manager.addGroup(m_name);
    input_manager.addKeybind(group_id,
        GLFW_KEY_ESCAPE,
        KeyState::PRESS_ONCE,
        []
        {
            SectionManager::get().clear();
        });
}

void MainMenuSection::update() noexcept { }

void MainMenuSection::render() noexcept
{
    m_layout.update(ImGui::GetMainViewport()->WorkPos, ImGui::GetMainViewport()->WorkSize);

    ImGui::SetNextWindowPos({m_layout.menu_x, m_layout.menu_y - 100.f});
    ImGui::Begin("MainMenuGameTitle", nullptr, m_layout.window_flags);
    {
        ImGui::Text("Top-down Shooter Game");
    }
    ImGui::End();

    ImGui::SetNextWindowPos({m_layout.menu_x, m_layout.menu_y});
    ImGui::SetNextWindowSize({m_layout.menu_w, m_layout.menu_h});
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {m_layout.button_w_s, m_layout.button_h_s});
    ImGui::Begin("MainMenu", nullptr, m_layout.window_flags);
    {
        if(ImGui::Button("New game", {m_layout.button_w, m_layout.button_h}))
        {
            spdlog::debug("Clicking 'New game'");
            auto& sm = SectionManager::get();
            sm.emplaceSection<DebugSection>();
        }
        else if(ImGui::Button("Map creator", {m_layout.button_w, m_layout.button_h}))
        {
            spdlog::debug("Clicking 'Map creator'");
            auto& sm = SectionManager::get();
            sm.emplaceSection<CreatorSection>();
        }
        else if(ImGui::Button("Settings", {m_layout.button_w, m_layout.button_h}))
        {
            spdlog::debug("Clicking 'Settings'");
            auto& sm = SectionManager::get();
            sm.emplaceSection<SettingsSection>();
        }
        else if(ImGui::Button("Exit", {m_layout.button_w, m_layout.button_h}))
        {
            spdlog::debug("Clicking 'Exit'");
            SectionManager::get().clear();
        }
    }
    ImGui::End();
    ImGui::PopStyleVar();
}
