#include "../../include/section/NewGameSection.hpp"

#include <input/InputManager.hpp>
#include <resource/ResourceManager.hpp>

#include "../../include/section/DebugSection.hpp"
#include "../../include/section/ForestMapSection.hpp"
#include "../../include/section/TutorialMapSection.hpp"
#include "../../include/section/WinterMapSection.hpp"
#include "../../include/ui/LowerNavigationBox.hpp"

NewGameSection::NewGameSection()
    : Section()
    , m_layout(ImGui::GetMainViewport()->WorkPos, ImGui::GetMainViewport()->WorkSize)
    , m_navLayout(ImGui::GetMainViewport()->WorkPos, ImGui::GetMainViewport()->WorkSize)
{
    m_name = "NewGameSection";
}

void NewGameSection::update() noexcept
{
    const ImGuiViewport& main_viewport = *ImGui::GetMainViewport();
    m_layout.update(main_viewport.WorkPos, main_viewport.WorkSize);
    m_navLayout.update(main_viewport.WorkPos, main_viewport.WorkSize);

    auto& input = InputManager::get();
    if(input.isPressedOnce(GLFW_KEY_ESCAPE))
    {
        SectionManager::get().popSection();
    }
    if(input.isPressedOnce(GLFW_KEY_F10))
    {
        m_showDebugLevel = !m_showDebugLevel;
    }
}

void NewGameSection::render() noexcept
{
    using res = ResourceManager;

    const auto& title_font = res::uiTitleFont;
    const auto& title_font_size = *res::uiTitleFontSize;

    auto title_font_guard = title_font->use();
    ImGui::SetNextWindowPos({m_layout.menuX, m_layout.menuY - title_font_size - 100.f});
    ImGui::SetNextWindowSize(
        {static_cast<float>(ResourceManager::window->getSize().x) - 200.f,
         title_font->get()->FontSize + 50.f});
    ImGui::Begin("NewGameMenuGameTitle", nullptr, m_layout.windowFlags);
    {
        auto next_y = ImGui::GetCursorScreenPos().y;
        ImGui::SetCursorScreenPos({m_layout.menuX + m_layout.buttonWS, next_y});
        ImGui::Text("MONOSHOT");
    }
    ImGui::End();
    title_font_guard.popFont();

    const auto& button_font = res::uiButtonFont;

    auto button_font_guard = button_font->use();
    ImGui::SetNextWindowPos({m_layout.menuX, m_layout.menuY});
    ImGui::SetNextWindowSize({m_layout.menuW, m_layout.menuH});
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {m_layout.buttonWS, m_layout.buttonHS});
    ImGui::Begin("NewGameMenu", nullptr, m_layout.windowFlags);
    {
        ImGui::SetCursorScreenPos(
            {m_layout.menuX + m_layout.buttonWS, m_layout.menuY + m_layout.buttonHS});
        if(ImGui::Button("Tutorial", {m_layout.buttonW, m_layout.buttonH}))
        {
            spdlog::debug("Clicking 'Tutorial'");
            auto& sm = SectionManager::get();
            sm.emplaceSection<TutorialMapSection>();
        }
        ImGui::SetCursorScreenPos(
            {m_layout.menuX + m_layout.buttonWS,
             m_layout.menuY + m_layout.buttonHS * 2 + m_layout.buttonH});
        if(ImGui::Button("Forest", {m_layout.buttonW, m_layout.buttonH}))
        {
            spdlog::debug("Clicking 'Forest'");
            auto& sm = SectionManager::get();
            sm.emplaceSection<ForestMapSection>();
        }
        ImGui::SetCursorScreenPos(
            {m_layout.menuX + m_layout.buttonWS,
             m_layout.menuY + m_layout.buttonHS * 3 + m_layout.buttonH * 2});
        if(ImGui::Button("Winter", {m_layout.buttonW, m_layout.buttonH}))
        {
            spdlog::debug("Clicking 'Winter'");
            auto& sm = SectionManager::get();
            sm.emplaceSection<WinterMapSection>();
        }
        if(m_showDebugLevel)
        {
            ImGui::SetCursorScreenPos(
                {m_layout.menuX + m_layout.buttonWS,
                 m_layout.menuY + m_layout.buttonHS * 4 + m_layout.buttonH * 3});
            if(ImGui::Button("Debug Level", {m_layout.buttonW, m_layout.buttonH}))
            {
                spdlog::debug("Clicking 'Debug Level'");
                auto& sm = SectionManager::get();
                sm.emplaceSection<DebugSection>();
            }
        }
    }
    ImGui::End();
    ImGui::PopStyleVar();

    UI::LowerNavigationBox(m_navLayout);
}
