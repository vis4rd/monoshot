#pragma once

#include "../section/SectionManager.hpp"


class Window final
{
    public:
    Window(const Window &) = delete;
    Window(Window &&) = delete;
    Window &operator=(const Window &) = delete;
    Window &operator=(Window &&) = delete;
    static Window &get();

    GLFWwindow *getGlfwWindow();
    const ImGuiIO &getImGuiIo() const;
    std::pair<int32_t, int32_t> getSize() const;
    const std::string &getTitle() const;

    bool isFullscreen() const;
    bool isMaximized() const;
    bool isVerticalSyncEnabled() const;

    void toggleFullscreen();

    void setSize(const std::pair<int32_t, int32_t> &new_size);
    void setTitle(const std::string &title);
    void setFullscreen(bool fullscreen = true);
    void setMaximized(bool maximized = true);
    void setVerticalSync(bool vsync = true);

    template<typename... UPDATEABLES>
    requires CUpdateablePack<UPDATEABLES...>
    bool update(UPDATEABLES &&...updateables) noexcept;

    template<typename... RENDERABLES>
    requires CRenderablePack<RENDERABLES...>
    void render(RENDERABLES &&...renderables) noexcept;

    private:
    Window();
    void initGLFW();
    void initImGui();
    void initGL();
    void initKeybinds();
    void terminate();

    private:
    GLFWwindow *m_window = nullptr;
    ImGuiIO m_io;
    std::string m_title = "Game";
    uint32_t m_width = 640;
    uint32_t m_height = 480;
    bool m_shouldClose = false;
    bool m_isFullscreen = false;
    bool m_isVSyncEnabled = true;
    SectionManager &m_sectionManager = SectionManager::get();
    Input &m_inputManager = Input::get();
};


template<typename... UPDATEABLES>
requires CUpdateablePack<UPDATEABLES...>
bool Window::update(UPDATEABLES &&...updateables) noexcept
{
    if(m_sectionManager.size() == 0)
    {
        spdlog::debug("SectionManager asks to close the window");
        m_shouldClose = true;
    }
    if(glfwWindowShouldClose(m_window))
    {
        spdlog::debug("GLFW asks to close the window");
        m_shouldClose = true;
        m_sectionManager.clear();
    }

    if(m_shouldClose)
    {
        this->terminate();
        spdlog::trace("Window::update() returns false");
        return false;
    }

    spdlog::trace("Handling inputs");
    m_inputManager.processGroup(m_window, "window");
    m_inputManager.processGroup(m_window, m_sectionManager.topSection().name());

    spdlog::trace("Polling GLFW events");
    glfwPollEvents();

    spdlog::trace("Updating updateables passed to Window::update()");
    if constexpr(sizeof...(updateables) > 0)
    {
        (updateables.update(), ...);
    }

    return true;
}

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
        static bool show_demo_window = false;
        ImGui::Begin("Debug Panel");

        ImGui::Checkbox("Demo Window", &show_demo_window);
        if(show_demo_window)
        {
            ImGui::ShowDemoWindow(&show_demo_window);
        }

        if(ImGui::Checkbox("Toggle VSYNC", &m_isVSyncEnabled))
        {
            this->setVerticalSync(m_isVSyncEnabled);
        }

        if(ImGui::Checkbox("Toggle fullscreen", &m_isFullscreen))
        {
            this->setFullscreen(m_isFullscreen);
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
