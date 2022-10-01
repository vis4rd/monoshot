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

void SettingsSection::update() noexcept { }

void SettingsSection::render() noexcept
{
    const ImGuiViewport& main_viewport = *ImGui::GetMainViewport();
    m_layout.update(main_viewport.WorkPos, main_viewport.WorkSize);
    m_navLayout.update(main_viewport.WorkPos, main_viewport.WorkSize);

    ImGui::SetNextWindowPos({m_layout.menu_x, m_layout.menu_y});
    ImGui::SetNextWindowSize({m_layout.menu_w, m_layout.menu_h});
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {m_layout.button_w_s, m_layout.button_h_s});
    ImGui::Begin("SettingsMenu", nullptr, m_layout.window_flags);
    {
        const auto& window_size = ResourceManager::window->getSize();
        const auto& window_w = window_size.x;
        const auto& window_h = window_size.y;
        std::string current_resolution = std::to_string(window_w) + "x" + std::to_string(window_h);
        ImGui::SetCursorScreenPos({m_layout.menu_x + m_layout.button_w_s, m_layout.menu_y + m_layout.button_h_s});
        if(Custom::ImGui::BeginCombo("Resolution", current_resolution.c_str(), {m_layout.button_w, m_layout.button_h}))
        {
            // const auto* modes = glfwGetVideoModes(glfwGetPrimaryMonitor(), &mode_count);
            const auto resolutions = ResourceManager::window->queryMonitorResolutions();
            int32_t mode_count = resolutions.size();

            std::vector<bool> states(mode_count);
            for(int32_t i = 0; i < mode_count; i++)
            {
                const auto& temp_w = resolutions[i].x;
                const auto& temp_h = resolutions[i].y;
                std::string temp_resolution = std::to_string(temp_w) + "x" + std::to_string(temp_h);
                if(ImGui::Selectable(temp_resolution.c_str(), states[i]))
                {
                    current_resolution = temp_resolution;
                    ResourceManager::window->setSize({temp_w, temp_h});
                }
            }
            ImGui::EndCombo();
        }

        if(ImGui::Button("Button 1", {m_layout.button_w, m_layout.button_h}))
        {
            spdlog::debug("Clicking Button 1");
        }
    }
    ImGui::End();
    ImGui::PopStyleVar();

    UI::LowerNavigationBox(m_navLayout);
}
