#include "../../include/section/SettingsSection.hpp"

SettingsSection::SettingsSection()
    : Section(), m_layout(ImGui::GetMainViewport()->WorkPos, ImGui::GetMainViewport()->WorkSize, {1840.f, 660.f}),
      m_navLayout(ImGui::GetMainViewport()->WorkPos, ImGui::GetMainViewport()->WorkSize, {1840.f, 110.f})
{
    m_name = "SettingsSection";
    auto& input_manager = InputManager::get();
    auto group_id = input_manager.addGroup(m_name);
    input_manager.addKeybind(group_id,
        GLFW_KEY_ESCAPE,
        GLFW_PRESS,
        []
        {
            SectionManager::get().popSection();
        });

    m_layout.menu_y = m_layout.viewport_y + m_layout.external_h_spacing;
    m_layout.menu_h = m_layout.viewport_h - m_navLayout.menu_h - (3.f * m_layout.external_h_spacing);
}

void SettingsSection::update() noexcept { }

void SettingsSection::render() noexcept
{
    const ImGuiViewport& main_viewport = *ImGui::GetMainViewport();
    m_layout.update(main_viewport.WorkPos, main_viewport.WorkSize);
    m_navLayout.update(main_viewport.WorkPos, main_viewport.WorkSize);
    m_layout.menu_y = m_layout.viewport_y + m_layout.external_h_spacing;
    m_layout.menu_h = m_layout.viewport_h - m_navLayout.menu_h - (3.f * m_layout.external_h_spacing);

    ImGui::SetNextWindowPos({m_layout.menu_x, m_layout.menu_y});
    ImGui::SetNextWindowSize({m_layout.menu_w, m_layout.menu_h});
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {m_layout.button_w_s, m_layout.button_h_s});
    ImGui::Begin("SettingsMenu", nullptr, m_layout.window_flags);
    {
        const auto& [window_w, window_h] = Window::get().getSize();
        std::string current_resolution = std::to_string(window_w) + "x" + std::to_string(window_h);
        if(Custom::ImGui::BeginCombo("Resolution", current_resolution.c_str(), {m_layout.button_w, m_layout.button_h}))
        {
            int32_t mode_count = 0;
            const auto* modes = glfwGetVideoModes(glfwGetPrimaryMonitor(), &mode_count);
            std::vector<bool> states(mode_count);
            for(int32_t i = 0; i < mode_count; i++)
            {
                const auto& temp_w = modes[i].width;
                const auto& temp_h = modes[i].height;
                std::string temp_resolution = std::to_string(temp_w) + "x" + std::to_string(temp_h);
                if(ImGui::Selectable(temp_resolution.c_str(), states[i]))
                {
                    current_resolution = temp_resolution;
                    Window::get().setSize({temp_w, temp_h});
                }
            }
            ImGui::EndCombo();
        }
        else if(ImGui::Button("Button 1", {m_layout.button_w, m_layout.button_h}))
        {
            spdlog::debug("Clicking Button 1");
        }
    }
    ImGui::End();
    ImGui::PopStyleVar();

    UI::LowerNavigationBox(m_navLayout);
}
