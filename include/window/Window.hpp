#pragma once

#include "../section/SectionManager.hpp"
#include "../utility/VertexArray.hpp"
#include "../utility/FrameBuffer.hpp"
#include "NativeWindow.hpp"

class Window final : public NativeWindow
{
    public:
    Window();
    Window(const std::string &title, std::uint32_t width, std::uint32_t height, bool fullscreen = true, bool vsync = true);
    ~Window();

    const ImGuiIO &getImGuiIo() const;
    FrameBuffer &getFramebuffer();
    glm::vec2 getMousePosition() const;

    bool isFullscreen() const;
    bool isMaximized() const;
    bool isVerticalSyncEnabled() const;

    void toggleFullscreen();

    void setSize(const std::pair<int32_t, int32_t> &new_size) override;
    void setFramebufferSize(const std::pair<int32_t, int32_t> &new_size);
    void setFullscreen(bool fullscreen = true);
    void setMaximized(bool maximized = true);
    void setMinimized(bool minimized = true);
    void setVerticalSync(bool vsync = true);
    void setRefreshRate(std::uint32_t hz);

    template<typename... UPDATEABLES>
    requires CUpdateablePack<UPDATEABLES...>
    bool update(UPDATEABLES &&...updateables) noexcept;

    template<typename... RENDERABLES>
    requires CRenderablePack<RENDERABLES...>
    void render(RENDERABLES &&...renderables) noexcept;

    private:
    void initImGui();

    private:
    ImGuiIO m_io;
    bool m_shouldClose = false;
    bool m_isFullscreen = false;
    bool m_isMaximized = false;
    bool m_isMinimized = false;
    bool m_isVSyncEnabled = false;
    SectionManager &m_sectionManager = SectionManager::get();
    InputManager &m_inputManager = InputManager::get();
    VertexArray screenVA;
    FrameBuffer screenFB;
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
        spdlog::trace("Window::update() returns false");
        return false;
    }

    spdlog::trace("Polling GLFW events");
    glfwPollEvents();

    spdlog::trace("Handling inputs");

    auto &input = InputManager::get();
    if(input.isPressedOnce(GLFW_KEY_F11))
    {
        spdlog::debug("on F11: window size = {}x{}", m_width, m_height);
        spdlog::debug("on F11: framebuffer size = {}x{}", screenFB.getSize().x, screenFB.getSize().y);
        this->toggleFullscreen();
        spdlog::debug("after F11: window size = {}x{}", m_width, m_height);
        spdlog::debug("after F11: framebuffer size = {}x{}", screenFB.getSize().x, screenFB.getSize().y);
    }
    if constexpr(Flag::DebugMode)
    {
        if(input.isPressedOnce(GLFW_KEY_APOSTROPHE))  // debugging purposes
        {
            static int break_count;
            spdlog::debug("======================= {} =========================", break_count++);
        }
    }
    if(SectionManager::get().size() == 1)
    {
        if(input.isPressedOnce(GLFW_KEY_ESCAPE))
        {
            m_shouldClose = true;
        }
    }

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
    if(m_isMinimized)
    {
        return;
    }

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Clear previous frame
    screenFB.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glViewport(0, 0, screenFB.getSize().x, screenFB.getSize().y);  // set framebuffer viewport to its size

    // Render
    /// Render my own stuff
    if constexpr(sizeof...(renderables) > 0)
    {
        (renderables.render(), ...);
    }

    // Debug panel
    if constexpr(Flag::DebugMode)
    {
        static bool show_debug_panel = true;
        static bool enable_vsync = m_isVSyncEnabled;
        static bool enable_fullscreen = m_isFullscreen;
        if(show_debug_panel)
        {
            static bool show_demo_window = false;
            ImGui::Begin("Debug Panel");
            {
                ImGui::Checkbox("Demo Window", &show_demo_window);
                if(show_demo_window)
                {
                    ImGui::ShowDemoWindow(&show_demo_window);
                }

                if(ImGui::Checkbox("Toggle VSYNC", &enable_vsync))
                {
                    this->setVerticalSync(enable_vsync);
                }

                if(ImGui::Checkbox("Toggle fullscreen", &enable_fullscreen))
                {
                    this->setFullscreen(enable_fullscreen);
                }

                ImGui::Text("Window size: (%d, %d)", m_width, m_height);
                ImGui::Text("Framebuffer size: (%d, %d)", screenFB.getSize().x, screenFB.getSize().y);
                ImGui::Text("Performance: [%.2fms] [%.0ffps]", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::Text("Mouse Position: Screen[%.2fx, %.2fy]", ImGui::GetMousePos().x, ImGui::GetMousePos().y);
            }
            ImGui::End();
        }
    }

    /// Render ImGui (UI) on top
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if(m_io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow *backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }

    screenFB.unbind();
    // commented for now, because technically whole texture is overdrawn
    // glClearColor(1.f, 1.f, 1.f, 1.f);
    // glClear(GL_COLOR_BUFFER_BIT);

    glViewport(0, 0, m_width, m_height);  // set the main viewport to window size

    // use screen shader
    ShaderManager::useShader("screen");
    screenVA.bind();
    glBindTexture(GL_TEXTURE_2D, screenFB.getColorID());
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    // Replace previous frame with the current one
    glfwSwapBuffers(m_window);
}
