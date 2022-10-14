#include "../../include/section/MainMenuSection.hpp"

#include "../../include/section/CreatorSection.hpp"
#include "../../include/utility/ResourceManager.hpp"

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
    using res = ResourceManager;
    const auto& title_font = res::uiTitleFont;
    const auto& title_font_size = *res::uiTitleFontSize;

    m_layout.update(ImGui::GetMainViewport()->WorkPos, ImGui::GetMainViewport()->WorkSize);

    auto title_font_guard = title_font->use();
    ImGui::SetNextWindowPos({m_layout.menu_x, m_layout.menu_y - title_font_size - 100.f});
    ImGui::SetNextWindowSize({ResourceManager::window->getSize().x - 200.f, title_font->get()->FontSize + 50.f});
    ImGui::Begin("MainMenuGameTitle", nullptr, m_layout.window_flags);
    {
        ImGui::Text("%s", m_titleText.c_str());
    }
    ImGui::End();
    title_font_guard.popFont();

    const auto& button_font = res::uiButtonFont;

    auto button_font_guard = button_font->use();
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
