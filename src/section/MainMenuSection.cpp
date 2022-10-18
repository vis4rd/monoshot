#include "../../include/section/MainMenuSection.hpp"

#include "../../include/section/CreatorSection.hpp"
#include "../../include/utility/ResourceManager.hpp"

MainMenuSection::MainMenuSection()
    : Section(),
      m_layout(ImGui::GetMainViewport()->WorkPos, ImGui::GetMainViewport()->WorkSize)
{
    m_name = "MainMenuSection";
}

void MainMenuSection::update() noexcept
{
    auto& input = InputManager::get();
    if(input.isPressedOnce(GLFW_KEY_ESCAPE))
    {
        SectionManager::get().popSection();
    }

    m_layout.update(ImGui::GetMainViewport()->WorkPos, ImGui::GetMainViewport()->WorkSize);
}

void MainMenuSection::render() noexcept
{
    using res = ResourceManager;
    const auto& title_font = res::uiTitleFont;
    const auto& title_font_size = *res::uiTitleFontSize;

    auto title_font_guard = title_font->use();
    ImGui::SetNextWindowPos({m_layout.menu_x, m_layout.menu_y - title_font_size - 100.f});
    ImGui::SetNextWindowSize({res::window->getSize().x - 200.f, title_font->get()->FontSize + 50.f});
    ImGui::Begin("MainMenuGameTitle", nullptr, m_layout.window_flags);
    {
        auto next_y = ImGui::GetCursorScreenPos().y;
        ImGui::SetCursorScreenPos({m_layout.menu_x + m_layout.button_w_s, next_y});
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
        ImGui::SetCursorScreenPos({m_layout.menu_x + m_layout.button_w_s, m_layout.menu_y + m_layout.button_h_s});
        if(ImGui::Button("New game", {m_layout.button_w, m_layout.button_h}))
        {
            spdlog::debug("Clicking 'New game'");
            auto& sm = SectionManager::get();
            sm.emplaceSection<NewGameSection>();
        }
        ImGui::SetCursorScreenPos({m_layout.menu_x + m_layout.button_w_s, ImGui::GetCursorScreenPos().y});
        if(ImGui::Button("Map creator", {m_layout.button_w, m_layout.button_h}))
        {
            spdlog::debug("Clicking 'Map creator'");
            auto& sm = SectionManager::get();
            sm.emplaceSection<CreatorSection>();
        }
        ImGui::SetCursorScreenPos({m_layout.menu_x + m_layout.button_w_s, ImGui::GetCursorScreenPos().y});
        if(ImGui::Button("Settings", {m_layout.button_w, m_layout.button_h}))
        {
            spdlog::debug("Clicking 'Settings'");
            auto& sm = SectionManager::get();
            sm.emplaceSection<SettingsSection>();
        }
        ImGui::SetCursorScreenPos({m_layout.menu_x + m_layout.button_w_s, ImGui::GetCursorScreenPos().y});
        if(ImGui::Button("Exit", {m_layout.button_w, m_layout.button_h}))
        {
            spdlog::debug("Clicking 'Exit'");
            SectionManager::get().clear();
        }
    }
    ImGui::End();
    ImGui::PopStyleVar();
}
