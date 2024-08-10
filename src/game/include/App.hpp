#pragma once

#include <GLFW/glfw3.h>
#include <config/ConfigLoader.hpp>
#include <config/StaticConfiguration.hpp>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <input/InputManager.hpp>
#include <opengl/shader/ShaderManager.hpp>
#include <opengl/target/RenderWindow.hpp>
#include <renderer/Renderer.hpp>
#include <section/SectionManager.hpp>
#include <time/Timer.hpp>
#include <traits/Renderable.hpp>
#include <traits/Updateable.hpp>
#include <ui/MainMenuStyle.hpp>

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
    InputManager& m_input;
    SectionManager& m_sectionManager;
};

void App::update(UpdateableTrait auto&&... updateables) noexcept
{
    if(m_sectionManager.size() == 0)
    {
        spdlog::debug("SectionManager asks to close the window");
        m_window->requestClose();
    }
    if(m_window->shouldClose())
    {
        spdlog::debug("Window should close, clearing SectionManager, returning...");
        m_sectionManager.clear();
        return;
    }

    glfwPollEvents();
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
            m_window->requestClose();
        }
    }

    if constexpr(sizeof...(updateables) > 0)
    {
        (updateables.update(), ...);
    }
}

void App::render(RenderableTrait auto&&... renderables) noexcept
{
    if(m_window->isMinimized() or m_window->shouldClose())
    {
        return;
    }

    m_window->prepareRender();

    // Render
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

    // mono::renderer::render(
    //     glm::ortho(
    //         0.f,
    //         static_cast<float>(m_window->getSize().x),
    //         static_cast<float>(m_window->getSize().y),
    //         0.f,
    //         0.1f,
    //         1000.f),
    //     glm::lookAt(
    //         glm::vec3{0.f, 0.f, 100.f},
    //         glm::vec3{0.f, 0.f, 0.f},
    //         glm::vec3{0.f, 1.f, 0.f}));

    m_window->render();
}
