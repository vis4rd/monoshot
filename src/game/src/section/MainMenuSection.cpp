#include "../../include/section/MainMenuSection.hpp"

#include <input/InputManager.hpp>
#include <resource/ResourceManager.hpp>
#include <section/SectionManager.hpp>

#include "../../include/section/CreatorSection.hpp"
#include "../../include/section/NewGameSection.hpp"
#include "../../include/section/SettingsSection.hpp"

MainMenuSection::MainMenuSection()
    : Section()
    , m_layout(ImGui::GetMainViewport()->WorkPos, ImGui::GetMainViewport()->WorkSize)
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
    ImGui::SetNextWindowPos({m_layout.menuX, m_layout.menuY - title_font_size - 100.f});
    ImGui::SetNextWindowSize(
        {static_cast<float>(res::window->getSize().x) - 200.f, title_font->get()->FontSize + 50.f});
    ImGui::Begin("MainMenuGameTitle", nullptr, m_layout.windowFlags);
    {
        auto next_y = ImGui::GetCursorScreenPos().y;
        ImGui::SetCursorScreenPos({m_layout.menuX + m_layout.buttonWS, next_y});
        ImGui::Text("%s", m_titleText.c_str());
    }
    ImGui::End();
    title_font_guard.popFont();

    const auto& button_font = res::uiButtonFont;

    auto button_font_guard = button_font->use();
    ImGui::SetNextWindowPos({m_layout.menuX, m_layout.menuY});
    ImGui::SetNextWindowSize({m_layout.menuW, m_layout.menuH});
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {m_layout.buttonWS, m_layout.buttonHS});
    ImGui::Begin("MainMenu", nullptr, m_layout.windowFlags);
    {
        ImGui::SetCursorScreenPos(
            {m_layout.menuX + m_layout.buttonWS, m_layout.menuY + m_layout.buttonHS});
        if(ImGui::Button("New game", {m_layout.buttonW, m_layout.buttonH}))
        {
            spdlog::debug("Clicking 'New game'");
            auto& sm = SectionManager::get();
            sm.emplaceSection<NewGameSection>();
        }
        ImGui::SetCursorScreenPos(
            {m_layout.menuX + m_layout.buttonWS, ImGui::GetCursorScreenPos().y});
        if(ImGui::Button("Map creator", {m_layout.buttonW, m_layout.buttonH}))
        {
            spdlog::debug("Clicking 'Map creator'");
            auto& sm = SectionManager::get();
            sm.emplaceSection<CreatorSection>();
        }
        ImGui::SetCursorScreenPos(
            {m_layout.menuX + m_layout.buttonWS, ImGui::GetCursorScreenPos().y});
        if(ImGui::Button("Settings", {m_layout.buttonW, m_layout.buttonH}))
        {
            spdlog::debug("Clicking 'Settings'");
            auto& sm = SectionManager::get();
            sm.emplaceSection<SettingsSection>();
        }
        ImGui::SetCursorScreenPos(
            {m_layout.menuX + m_layout.buttonWS, ImGui::GetCursorScreenPos().y});
        if(ImGui::Button("Exit", {m_layout.buttonW, m_layout.buttonH}))
        {
            spdlog::debug("Clicking 'Exit'");
            SectionManager::get().clear();
        }
    }
    ImGui::End();
    ImGui::PopStyleVar();
}
