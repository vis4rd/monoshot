#pragma once

#include <GLFW/glfw3.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <input/Input.hpp>
#include <mono/dev_ui/DevUI.hpp>
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
    App(const std::string& window_title);
    App(const App&) = delete;
    App(App&&) = delete;
    ~App() noexcept;

    App& operator=(const App&) = delete;
    App& operator=(App&&) = delete;

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
    std::shared_ptr<mono::gl::RenderWindow> m_window;
    std::shared_ptr<Timer> m_timer;
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

    mono::input::pollEvents();
    if(mono::input::isPressedOnce(GLFW_KEY_F11))
    {
        auto size = m_window->getSize();
        spdlog::debug("on F11: window size = {}x{}", size.x, size.y);

        m_window->toggleFullscreen();
        size = m_window->getSize();
        spdlog::debug("after F11: window size = {}x{}", size.x, size.y);
    }
    if(m_sectionManager.size() == 1)
    {
        if(mono::input::isPressedOnce(GLFW_KEY_ESCAPE))
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

    mono::dev_ui::render();

    mono::renderer::render();

    m_window->render();
}
