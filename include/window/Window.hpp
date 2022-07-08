#pragma once

#include "../section/MainMenuSection.hpp"

#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>
#include <string>


class Window final
{
    public:
    Window() = default;
    Window(const std::string &window_title, uint32_t width, uint32_t height);
    ~Window() noexcept = default;

    GLFWwindow *getWindow();
    const ImGuiIO &getImGuiIo() const;
    glm::uvec2 getSize() const;
    const std::string &getTitle() const;

    bool isMaximized() const;
    bool isVerticalSyncEnabled() const;

    void setTitle(const std::string &title);
    void setFullscreen(bool fullscreen = true);
    void setMaximized(bool maximized = true);
    void setVerticalSync(bool vsync = true);

    bool update() noexcept;

    template<typename... RENDERABLES>
    requires CRenderablePack<RENDERABLES...>
    void render(RENDERABLES &&...renderables) noexcept;

    private:
    void terminate();

    private:
    GLFWwindow *m_window = nullptr;
    ImGuiIO m_io;
    std::string m_title{};
    uint32_t m_width = 0;
    uint32_t m_height = 0;
};


template<typename... RENDERABLES>
requires CRenderablePack<RENDERABLES...>
void Window::render(RENDERABLES &&...renderables) noexcept
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    static bool show_debug_panel = true;
    static glm::vec4 clear_color = glm::vec4(0.f, 0.f, 0.f, 1.f);
    // Debug panel
    if(show_debug_panel)
    {
        static bool vsync = true;
        static bool show_demo_window = false;
        static bool fullscreen = false;
        ImGui::Begin("Debug Panel");

        ImGui::Checkbox("Demo Window", &show_demo_window);
        if(show_demo_window)
        {
            ImGui::ShowDemoWindow(&show_demo_window);
        }

        if(ImGui::Checkbox("Toggle VSYNC", &vsync))
        {
            this->setVerticalSync(vsync);
        }

        if(ImGui::Checkbox("Toggle fullscreen", &fullscreen))
        {
            this->setFullscreen(fullscreen);
            this->setVerticalSync(vsync);
        }

        if(ImGui::ColorEdit3("clear color", (float *)&clear_color))
        {
            glClearColor(clear_color.r * clear_color.a,
                clear_color.g * clear_color.a,
                clear_color.b * clear_color.a,
                clear_color.a);
        }

        ImGui::Text("Performance: %.3f ms,  %.1f fps", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        ImGui::End();
    }

    // Clear previous frame
    glClear(GL_COLOR_BUFFER_BIT);

    // Render
    //// Render my own stuff
    if constexpr(sizeof...(renderables) > 0)
    {
        (renderables.render(), ...);
    }

    //// Render ImGui (UI) on top
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if(m_io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow *backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }

    // Replace previous frame with the current one
    glfwSwapBuffers(m_window);
}
