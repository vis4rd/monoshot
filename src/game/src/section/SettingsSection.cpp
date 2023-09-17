#include "../../include/section/SettingsSection.hpp"

#include <audio/AudioManager.hpp>
#include <resource/ResourceManager.hpp>
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
    ImGui::SetNextWindowPos({m_layout.menu_x, m_layout.menu_y});
    ImGui::SetNextWindowSize({m_layout.menu_w, m_layout.menu_h});
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {m_layout.button_w_s, m_layout.button_h_s});
    ImGui::Begin("SettingsMenu", nullptr, m_layout.window_flags);
    {
        // Resolution
        const auto& window_size = window->getSize();
        const auto& window_w = window_size.x;
        const auto& window_h = window_size.y;
        std::string current_resolution = std::to_string(window_w) + "x" + std::to_string(window_h);
        ImGui::SetCursorScreenPos(
            {m_layout.menu_x + m_layout.button_w_s, m_layout.menu_y + m_layout.button_h_s});
        if(Custom::ImGui::BeginCombo(
               "Resolution",
               current_resolution.c_str(),
               {m_layout.button_w, m_layout.button_h}))
        {
            const auto resolutions = window->queryMonitorResolutions();
            const int32_t mode_count = resolutions.size();

            std::vector<bool> states(mode_count);
            for(int32_t i = 0; i < mode_count; i++)
            {
                const auto& temp_w = resolutions[i].x;
                const auto& temp_h = resolutions[i].y;
                std::string temp_resolution = std::to_string(temp_w) + "x" + std::to_string(temp_h);
                if(ImGui::Selectable(temp_resolution.c_str(), states[i]))
                {
                    current_resolution = temp_resolution;
                    window->setSize({temp_w, temp_h});
                }
            }
            ImGui::EndCombo();
        }

        // Refresh Rate
        ImGui::BeginDisabled(!window->isFullscreen());
        std::string current_refresh_rate = std::to_string(window->getRefreshRate()) + "Hz";
        auto next_y = ImGui::GetCursorScreenPos().y;
        ImGui::SetCursorScreenPos({m_layout.menu_x + m_layout.button_w_s, next_y});
        if(Custom::ImGui::BeginCombo(
               "Rate ",
               current_refresh_rate.c_str(),
               {m_layout.button_w, m_layout.button_h}))
        {
            const auto refresh_rates = window->queryMonitorRefreshRates();
            const int32_t mode_count = refresh_rates.size();

            std::vector<bool> states(mode_count);
            for(int32_t i = 0; i < mode_count; i++)
            {
                const auto refresh_rate = refresh_rates[i];
                std::string temp_refresh_rate = std::to_string(refresh_rate) + "Hz";
                if(ImGui::Selectable(temp_refresh_rate.c_str(), states[i]))
                {
                    current_refresh_rate = temp_refresh_rate;
                    window->setRefreshRate(refresh_rate);
                }
            }
            ImGui::EndCombo();
        }
        ImGui::EndDisabled();

        // Fullscreen
        std::string current_window_mode = window->isFullscreen() ? "Fullscreen" : "Windowed";
        next_y = ImGui::GetCursorScreenPos().y;
        ImGui::SetCursorScreenPos({m_layout.menu_x + m_layout.button_w_s, next_y});
        if(Custom::ImGui::BeginCombo(
               "Window Mode",
               current_window_mode.c_str(),
               {m_layout.button_w, m_layout.button_h}))
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
        ImGui::SetCursorScreenPos({m_layout.menu_x + m_layout.button_w_s, next_y});
        if(Custom::ImGui::BeginCombo(
               "Vertical Sync",
               current_vertical_sync.c_str(),
               {m_layout.button_w, m_layout.button_h}))
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
        ImGui::SetCursorScreenPos({m_layout.menu_x + m_layout.button_w_s, next_y});
        ImGui::SetNextItemWidth(m_layout.button_w);
        auto& limiter = res::framerateLimiter;
        static std::int32_t local_limit = limiter->getLimit();
        ImGui::BeginDisabled(window->isVerticalSyncEnabled());
        if(ImGui::SliderInt("Max FPS Limit", &local_limit, 60, 1000, "%d FPS"))
        {
            limiter->setLimit(local_limit);
        }
        ImGui::EndDisabled();

        // Master mixer volume
        auto& audio = AudioManager::get();
        next_y = ImGui::GetCursorScreenPos().y;
        ImGui::SetCursorScreenPos({m_layout.menu_x + m_layout.button_w_s, next_y});
        ImGui::SetNextItemWidth(m_layout.button_w);
        static std::int32_t local_master_volume =
            static_cast<int>(audio.getMixerMasterVolume() * 100);
        if(ImGui::SliderInt("Master Volume", &local_master_volume, 0, 100, "%d%"))
        {
            audio.setMixerMasterVolume(local_master_volume / 100.f);
        }

        // Music mixer volume
        next_y = ImGui::GetCursorScreenPos().y;
        ImGui::SetCursorScreenPos({m_layout.menu_x + m_layout.button_w_s, next_y});
        ImGui::SetNextItemWidth(m_layout.button_w);
        static std::int32_t local_music_volume =
            static_cast<int>(audio.getMixerMusicVolume() * 100);
        if(ImGui::SliderInt("Music Volume", &local_music_volume, 0, 100, "%d%"))
        {
            audio.setMixerMusicVolume(local_music_volume / 100.f);
        }

        // SFX mixer volume
        next_y = ImGui::GetCursorScreenPos().y;
        ImGui::SetCursorScreenPos({m_layout.menu_x + m_layout.button_w_s, next_y});
        ImGui::SetNextItemWidth(m_layout.button_w);
        static std::int32_t local_sfx_volume = static_cast<int>(audio.getMixerSfxVolume() * 100);
        if(ImGui::SliderInt("SFX Volume", &local_sfx_volume, 0, 100, "%d%"))
        {
            audio.setMixerSfxVolume(local_sfx_volume / 100.f);
        }
    }
    ImGui::End();
    ImGui::PopStyleVar();

    UI::LowerNavigationBox(m_navLayout);
}
