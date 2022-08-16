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
    void initKeybinds();
    void initFramebuffer();
    void initImGui();

    private:
    ImGuiIO m_io;
    bool m_shouldClose = false;
    bool m_isFullscreen = true;
    bool m_isVSyncEnabled = true;
    SectionManager &m_sectionManager = SectionManager::get();
    InputManager &m_inputManager = InputManager::get();

    std::uint32_t m_fbo = 0u;
    std::uint32_t m_fbColor = 0u;
    std::uint32_t m_vao = 0u;
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

    spdlog::trace("Handling inputs");
    m_inputManager.processGroup(m_window, m_sectionManager.topSection().name());
    m_inputManager.processGroup(m_window, "window");

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

    // Clear previous frame
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glClearColor(0.3f, 0.3f, 0.3f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Render
    //// Render my own stuff
    if constexpr(sizeof...(renderables) > 0)
    {
        (renderables.render(), ...);
    }

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
            glClearColor(clear_color.r * clear_color.a, clear_color.g * clear_color.a, clear_color.b * clear_color.a, clear_color.a);
        }

        ImGui::Text("Performance: [%.2fms] [%.0ffps]", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Text("Mouse Position: Screen[%.2fx, %.2fy]", ImGui::GetMousePos().x, ImGui::GetMousePos().y);

        ImGui::End();
    }

    glViewport(0, 0, m_width, m_height);
    glScissor(0, 0, m_width, m_height);

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

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // m_fbo.unbind();
    glClearColor(1.f, 1.f, 1.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    // use screen shader
    ShaderManager::useShader("screen");
    glBindVertexArray(m_vao);
    // glBindTexture(GL_TEXTURE_2D, m_fbo.getColorID());
    glBindTexture(GL_TEXTURE_2D, m_fbColor);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    // Replace previous frame with the current one
    glfwSwapBuffers(m_window);
}
