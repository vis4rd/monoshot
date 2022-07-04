#include "../include/App.hpp"

App::App(const std::string& window_title, uint32_t width, uint32_t height) : m_window(window_title, width, height) { }

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

void App::terminate(int code) noexcept
{
    std::exit(code);
}
