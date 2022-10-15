#include "../../include/section/NewGameSection.hpp"

#include "../../include/utility/ResourceManager.hpp"

NewGameSection::NewGameSection()
    : Section(),
      m_layout(ImGui::GetMainViewport()->WorkPos, ImGui::GetMainViewport()->WorkSize),
      m_navLayout(ImGui::GetMainViewport()->WorkPos, ImGui::GetMainViewport()->WorkSize)
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
}

void NewGameSection::render() noexcept
{
    using res = ResourceManager;

    const auto& title_font = res::uiTitleFont;
    const auto& title_font_size = *res::uiTitleFontSize;

    auto title_font_guard = title_font->use();
    ImGui::SetNextWindowPos({m_layout.menu_x, m_layout.menu_y - title_font_size - 100.f});
    ImGui::SetNextWindowSize({ResourceManager::window->getSize().x - 200.f, title_font->get()->FontSize + 50.f});
    ImGui::Begin("NewGameMenuGameTitle", nullptr, m_layout.window_flags);
    {
        auto next_y = ImGui::GetCursorScreenPos().y;
        ImGui::SetCursorScreenPos({m_layout.menu_x + m_layout.button_w_s, next_y});
        ImGui::Text("MONOSHOT");
    }
    ImGui::End();
    title_font_guard.popFont();

    const auto& button_font = res::uiButtonFont;

    auto button_font_guard = button_font->use();
    ImGui::SetNextWindowPos({m_layout.menu_x, m_layout.menu_y});
    ImGui::SetNextWindowSize({m_layout.menu_w, m_layout.menu_h});
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {m_layout.button_w_s, m_layout.button_h_s});
    ImGui::Begin("NewGameMenu", nullptr, m_layout.window_flags);
    {
        ImGui::SetCursorScreenPos({m_layout.menu_x + m_layout.button_w_s, m_layout.menu_y + m_layout.button_h_s});
        if(ImGui::Button("Debug Level", {m_layout.button_w, m_layout.button_h}))
        {
            spdlog::debug("Clicking 'Debug Level'");
            auto& sm = SectionManager::get();
            sm.emplaceSection<DebugSection>();
        }
    }
    ImGui::End();
    ImGui::PopStyleVar();

    UI::LowerNavigationBox(m_navLayout);
}
