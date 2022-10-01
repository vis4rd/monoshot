#include "../../include/section/SettingsSection.hpp"
#include "../../include/utility/ResourceManager.hpp"

SettingsSection::SettingsSection()
    : Section(),
      m_layout(ImGui::GetMainViewport()->WorkPos, ImGui::GetMainViewport()->WorkSize),
      m_navLayout(ImGui::GetMainViewport()->WorkPos, ImGui::GetMainViewport()->WorkSize)
{
    m_name = "SettingsSection";
    auto& input_manager = InputManager::get();
    auto group_id = input_manager.addGroup(m_name);
    input_manager.addKeybind(group_id,
        GLFW_KEY_ESCAPE,
        KeyState::PRESS_ONCE,
        []
        {
            SectionManager::get().popSection();
        });
}

void SettingsSection::update() noexcept
{
    const ImGuiViewport& main_viewport = *ImGui::GetMainViewport();
    m_layout.update(main_viewport.WorkPos, main_viewport.WorkSize);
    m_navLayout.update(main_viewport.WorkPos, main_viewport.WorkSize);
}

void SettingsSection::render() noexcept
{
    auto& window = ResourceManager::window;
    ImGui::SetNextWindowPos({m_layout.menu_x, m_layout.menu_y});
    ImGui::SetNextWindowSize({m_layout.menu_w, m_layout.menu_h});
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {m_layout.button_w_s, m_layout.button_h_s});
    ImGui::Begin("SettingsMenu", nullptr, m_layout.window_flags);
    {
        const auto& window_size = window->getSize();
        const auto& window_w = window_size.x;
        const auto& window_h = window_size.y;
        std::string current_resolution = std::to_string(window_w) + "x" + std::to_string(window_h);
        ImGui::SetCursorScreenPos({m_layout.menu_x + m_layout.button_w_s, m_layout.menu_y + m_layout.button_h_s});
        if(Custom::ImGui::BeginCombo("Resolution", current_resolution.c_str(), {m_layout.button_w, m_layout.button_h}))
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

        ImGui::BeginDisabled(!window->isFullscreen());
        std::string current_refresh_rate = std::to_string(window->getRefreshRate()) + "Hz";
        auto next_y = ImGui::GetCursorScreenPos().y;
        ImGui::SetCursorScreenPos({m_layout.menu_x + m_layout.button_w_s, next_y});
        if(Custom::ImGui::BeginCombo("Refresh Rate", current_refresh_rate.c_str(), {m_layout.button_w, m_layout.button_h}))
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


        // next_y = ImGui::GetCursorScreenPos().y;
        // ImGui::SetCursorScreenPos({m_layout.menu_x + m_layout.button_w_s, next_y});
        // if(ImGui::Button("Button 1", {m_layout.button_w, m_layout.button_h}))
        // {
        //     spdlog::debug("Clicking Button 1");
        // }
    }
    ImGui::End();
    ImGui::PopStyleVar();

    UI::LowerNavigationBox(m_navLayout);
}
