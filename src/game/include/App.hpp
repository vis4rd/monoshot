#pragma once

#include <GLFW/glfw3.h>
#include <config/ConfigLoader.hpp>
#include <opengl/target/RenderWindow.hpp>
#include <time/FramerateLimiter.hpp>
#include <time/Timer.hpp>
#include <traits/Updateable.hpp>
#include <ui/MainMenuStyle.hpp>
#include <window/Window.hpp>

class App final
{
    public:
    App(const std::string& window_title, uint32_t width, uint32_t height);
    App(const App&) = delete;
    App(App&&) = delete;
    ~App() noexcept;

    App& operator=(const App&) = delete;
    App& operator=(App&&) = delete;

    static void initLogger() noexcept;
    void initTextures() noexcept;
    void initFonts() noexcept;

    void update(UpdateableTrait auto&&... updateables) noexcept;
    void render(RenderableTrait auto&&... renderables) noexcept;
    void run() noexcept;
    void terminate(int code = 0) noexcept;

    private:
    void destroyTextures() noexcept;
    void destroyFonts() noexcept;

    private:
    mono::ConfigLoader& m_configLoader;
    std::shared_ptr<mono::gl::RenderWindow> m_window;
    std::shared_ptr<Timer> m_timer;
    std::shared_ptr<FramerateLimiter> m_limiter;
    InputManager& m_input;
    SectionManager& m_sectionManager;
    bool m_shouldClose = false;
};

void App::update(UpdateableTrait auto&&... updateables) noexcept
{
    if(m_sectionManager.size() == 0)
    {
        spdlog::debug("SectionManager asks to close the window");
        m_shouldClose = true;
    }
    if(glfwWindowShouldClose(m_window->getNativeWindow()))
    {
        spdlog::debug("GLFW asks to close the window");
        m_shouldClose = true;
        m_sectionManager.clear();
    }

    if(m_shouldClose)
    {
        spdlog::debug("App Update: close flag set to true, returning...");
        return;
    }

    // spdlog::trace("Polling GLFW events");
    glfwPollEvents();

    // spdlog::trace("Handling inputs");

    auto& input = InputManager::get();
    if(input.isPressedOnce(GLFW_KEY_F11))
    {
        auto size = m_window->getSize();
        spdlog::debug("on F11: window size = {}x{}", size.x, size.y);

        m_window->toggleFullscreen();
        size = m_window->getSize();
        spdlog::debug("after F11: window size = {}x{}", size.x, size.y);
    }
    if constexpr(mono::config::constant::debugMode)
    {
        if(input.isPressedOnce(GLFW_KEY_APOSTROPHE))  // debugging purposes
        {
            static int break_count;
            spdlog::debug("======================= {} =========================", break_count++);
        }
    }
    if(m_sectionManager.size() == 1)
    {
        if(input.isPressedOnce(GLFW_KEY_ESCAPE))
        {
            m_shouldClose = true;
        }
    }

    // spdlog::trace("Updating updateables passed to Window::update()");
    if constexpr(sizeof...(updateables) > 0)
    {
        (updateables.update(), ...);
    }
}

void App::render(RenderableTrait auto&&... renderables) noexcept
{
    if(m_shouldClose)
    {
        return;
    }
    if(m_window->isMinimized())
    {
        return;
    }

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Set window as the current rendering target
    m_window->activate();

    // Render
    /// Render my own stuff
    if constexpr(sizeof...(renderables) > 0)
    {
        (renderables.render(), ...);
    }

    // Debug panel
    if constexpr(mono::config::constant::debugMode)
    {
        static bool show_debug_panel = true;
        static bool enable_vsync = m_window->isVerticalSyncEnabled();
        static bool enable_fullscreen = m_window->isFullscreen();
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
                    m_window->setVerticalSync(enable_vsync);
                }

                if(ImGui::Checkbox("Toggle fullscreen", &enable_fullscreen))
                {
                    m_window->setFullscreen(enable_fullscreen);
                }

                const auto size = m_window->getSize();
                ImGui::Text("Window size: (%d, %d)", size.x, size.y);
                ImGui::Text(
                    "Performance: [%.2fms] [%.0ffps]",
                    1000.0f / ImGui::GetIO().Framerate,
                    ImGui::GetIO().Framerate);
                ImGui::Text(
                    "Mouse Position: Screen[%.2fx, %.2fy]",
                    ImGui::GetMousePos().x,
                    ImGui::GetMousePos().y);
            }
            ImGui::End();
        }
    }

    /// Render ImGui (UI) on top
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    const auto& io = ImGui::GetIO();
    if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }

    m_window->deactivate();
    // glViewport(0, 0, m_width, m_height);  // set the main viewport to window size

    mono::gl::ShaderManager::get().useShader("render_target");
    m_window->render();

    glfwSwapBuffers(m_window->getNativeWindow());
}
