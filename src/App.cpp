#include "../include/App.hpp"

App::App(const std::string& window_title, uint32_t width, uint32_t height)
    : m_window(window_title, width, height), m_input(), m_sectionManager()
{
    auto key_group = m_input.addGroup("main");
    m_input.addKeybind(
        "main",
        GLFW_KEY_ESCAPE,
        GLFW_PRESS,
        [](GLFWwindow* window)
        {
            glfwSetWindowShouldClose(window, true);
        },
        m_window.getWindow());

    m_input.addKeybind("main",
        GLFW_KEY_ENTER,
        GLFW_PRESS,
        []()
        {
            std::cout << "Hooray!\n";
        });

    auto menu = std::make_unique<MainMenuSection>();
    m_sectionManager.addSection(std::move(menu));
}

App::~App() noexcept
{
    this->terminate(EXIT_SUCCESS);
}

Window& App::getWindow()
{
    return m_window;
}

const Window& App::getWindow() const
{
    return m_window;
}

void App::run() noexcept
{
    while(m_window.update())
    {
        m_input.processGroup(m_window.getWindow(), "main");
        m_window.render(m_sectionManager);
    }
}

void App::terminate(int code) noexcept
{
    std::exit(code);
}
