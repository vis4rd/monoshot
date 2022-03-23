#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <stdexcept>
#include <iostream>

class App
{
    public:
    App(const std::string& window_title, uint32_t width, uint32_t height);
    virtual ~App() noexcept;

    GLFWwindow* getWindow();

    void setWindowTitle(const std::string& title);
    void setFullscreen(const bool fullscreen = true);

    private:
    void terminate(int code = 0) noexcept;

    private:
    GLFWwindow* m_window;
};
