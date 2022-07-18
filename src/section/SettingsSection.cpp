#include "../../include/section/SettingsSection.hpp"

SettingsSection::SettingsSection() : Section()
{
    m_name = "SettingsSection";
    auto& input_manager = Input::get();
    auto group_id = input_manager.addGroup(m_name);
    input_manager.addKeybind(group_id,
        GLFW_KEY_ESCAPE,
        GLFW_PRESS,
        []
        {
            SectionManager::get().popSection();
        });
}

void SettingsSection::update() noexcept { }

void SettingsSection::render() noexcept
{
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoScrollbar;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoNav;
    // window_flags |= ImGuiWindowFlags_NoBackground;  // uncomment when everything setup
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
    window_flags |= ImGuiWindowFlags_NoDocking;

    const ImGuiViewport& main_viewport = *ImGui::GetMainViewport();
    auto& style = ImGui::GetStyle();

    auto base_viewport_w = 1920.f;
    auto base_viewport_h = 1080.f;
    auto base_menu_w = 1840.f;
    auto base_menu_h = 660.f;
    auto base_button_w = 256.f;
    auto base_button_h = 64.f;
    auto base_h_spacing = 8.f;
    auto base_external_w_spacing = 40.f;
    auto base_external_h_spacing = 40.f;


    auto& [viewport_w, viewport_h] = main_viewport.WorkSize;
    auto& [viewport_x, viewport_y] = main_viewport.WorkPos;
    auto scale_w = viewport_w / base_viewport_w;
    auto scale_h = viewport_h / base_viewport_h;

    auto external_h_spacing = 40.f * scale_h;
    auto external_w_spacing = 40.f * scale_w;
    auto menu_w = base_menu_w * scale_w;
    // auto menu_h = base_menu_h * scale_h;
    auto menu_h = viewport_h - (110.f * scale_h) - (3 * external_h_spacing);
    auto menu_x = viewport_x + external_w_spacing;
    auto menu_y = viewport_y + external_h_spacing;
    auto button_w = base_button_w * scale_w;
    auto button_h = base_button_h * scale_h;
    auto button_w_s = base_h_spacing * scale_w;
    auto button_h_s = base_h_spacing * scale_h;

    ImGui::SetNextWindowPos({menu_x, menu_y});
    ImGui::SetNextWindowSize({menu_w, menu_h});
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {button_w_s, button_h_s});
    ImGui::Begin("SettingsMenu", nullptr, window_flags);
    {
        const auto& [window_w, window_h] = Window::get().getSize();
        std::string current_resolution = std::to_string(window_w) + "x" + std::to_string(window_h);
        ImGui::PushItemWidth(button_w);  // why should need this, check Custom::ImGui::BeginCombo!
        if(Custom::ImGui::BeginCombo("Resolution", current_resolution.c_str(), {button_w, button_h}))
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
            ImGui::PopItemWidth();
        }
        if(ImGui::Button("Button 1", {button_w, button_h}))
        {
            spdlog::debug("Clicking Button 1");
        }
        else if(ImGui::Button("Button 2", {button_w, button_h}))
        {
            spdlog::debug("Clicking Button 2");
        }
        else if(ImGui::Button("Button 3", {button_w, button_h}))
        {
            spdlog::debug("Clicking Button 3");
        }
    }
    ImGui::End();
    ImGui::PopStyleVar();

    UI::LowerNavigationBox();
}
