#include "../../include/section/SettingsSection.hpp"

#include <input/InputManager.hpp>
#include <resource/ResourceManager.hpp>
#include <section/SectionManager.hpp>
#include <ui/external/BeginCombo.hpp>

#include "../../include/ui/LowerNavigationBox.hpp"

SettingsSection::SettingsSection()
    : Section()
    , m_layout(ImGui::GetMainViewport()->WorkPos, ImGui::GetMainViewport()->WorkSize)
    , m_navLayout(ImGui::GetMainViewport()->WorkPos, ImGui::GetMainViewport()->WorkSize)
{
    m_name = "SettingsSection";
}

void SettingsSection::update() noexcept
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

void SettingsSection::render() noexcept
{
    using res = ResourceManager;
    auto& window = res::window;
    const auto& button_font = res::uiButtonFont;

    auto button_font_guard = button_font->use();
    ImGui::SetNextWindowPos({m_layout.menuX, m_layout.menuY});
    ImGui::SetNextWindowSize({m_layout.menuW, m_layout.menuH});
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {m_layout.buttonWS, m_layout.buttonHS});
    ImGui::Begin("SettingsMenu", nullptr, m_layout.windowFlags);
    {
        // Resolution
        const auto& window_size = window->getSize();
        const auto& window_w = window_size.x;
        const auto& window_h = window_size.y;
        std::string current_resolution = std::to_string(window_w) + "x" + std::to_string(window_h);
        ImGui::SetCursorScreenPos(
            {m_layout.menuX + m_layout.buttonWS, m_layout.menuY + m_layout.buttonHS});
        if(Custom::ImGui::BeginCombo(
               "Resolution",
               current_resolution.c_str(),
               {m_layout.buttonW, m_layout.buttonH}))
        {
            const auto resolutions = mono::gl::RenderWindow::queryMonitorResolutions();
            const std::size_t mode_count = resolutions.size();

            std::vector<bool> states(mode_count);
            for(int32_t i = 0; i < mode_count; i++)
            {
                const auto& temp_w = resolutions[i].x;
                const auto& temp_h = resolutions[i].y;
                std::string temp_resolution = std::to_string(temp_w) + "x" + std::to_string(temp_h);
                if(ImGui::Selectable(temp_resolution.c_str(), states[i]))
                {
                    current_resolution = temp_resolution;
                    window->setSize(temp_w, temp_h);
                }
            }
            ImGui::EndCombo();
        }

        // Fullscreen
        std::string current_window_mode = window->isFullscreen() ? "Fullscreen" : "Windowed";
        auto next_y = ImGui::GetCursorScreenPos().y;
        ImGui::SetCursorScreenPos({m_layout.menuX + m_layout.buttonWS, next_y});
        if(Custom::ImGui::BeginCombo(
               "Window Mode",
               current_window_mode.c_str(),
               {m_layout.buttonW, m_layout.buttonH}))
        {
            if(ImGui::Selectable("Fullscreen", window->isFullscreen()))
            {
                current_window_mode = "Fullscreen";
                window->setFullscreen(true);
            }
            else if(ImGui::Selectable("Windowed", !window->isFullscreen()))
            {
                current_window_mode = "Windowed";
                window->setFullscreen(false);
            }
            ImGui::EndCombo();
        }

        // Vertical Sync
        std::string current_vertical_sync =
            window->isVerticalSyncEnabled() ? "Enabled" : "Disabled";
        next_y = ImGui::GetCursorScreenPos().y;
        ImGui::SetCursorScreenPos({m_layout.menuX + m_layout.buttonWS, next_y});
        if(Custom::ImGui::BeginCombo(
               "Vertical Sync",
               current_vertical_sync.c_str(),
               {m_layout.buttonW, m_layout.buttonH}))
        {
            if(ImGui::Selectable("Enabled", window->isVerticalSyncEnabled()))
            {
                current_window_mode = "Enabled";
                window->setVerticalSync(true);
            }
            else if(ImGui::Selectable("Disabled", !window->isVerticalSyncEnabled()))
            {
                current_window_mode = "Disabled";
                window->setVerticalSync(false);
            }
            ImGui::EndCombo();
        }

        // Max FPS Limit
        next_y = ImGui::GetCursorScreenPos().y;
        ImGui::SetCursorScreenPos({m_layout.menuX + m_layout.buttonWS, next_y});
        ImGui::SetNextItemWidth(m_layout.buttonW);
        auto& limiter = res::framerateLimiter;
        static std::int32_t local_limit = limiter->getLimit();
        ImGui::BeginDisabled(window->isVerticalSyncEnabled());
        if(ImGui::SliderInt("Max FPS Limit", &local_limit, 60, 1000, "%d FPS"))
        {
            limiter->setLimit(local_limit);
        }
        ImGui::EndDisabled();
    }
    ImGui::End();
    ImGui::PopStyleVar();

    UI::LowerNavigationBox(m_navLayout);
}
