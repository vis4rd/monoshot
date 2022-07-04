#pragma once

#include <iostream>
#include <memory>
#include <stdexcept>

#include "window/Window.hpp"

class App
{
    public:
    App(const std::string& window_title, uint32_t width, uint32_t height);
    virtual ~App() noexcept;

    Window& getWindow();
    const Window& getWindow() const;

    private:
    void terminate(int code = 0) noexcept;

    private:
    Window m_window;
};
