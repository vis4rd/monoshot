#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glm/glm.hpp>
#include <iostream>
#include <string>

class Window final
{
    public:
    Window() = default;
    Window(const std::string &window_title, uint32_t width, uint32_t height);
    ~Window() noexcept = default;

    GLFWwindow *getWindow();
    const ImGuiIO &getImGuiIo() const;
    glm::uvec2 getSize() const;
    const std::string &getTitle() const;

    bool isVerticalSyncEnabled() const;

    void setTitle(const std::string &title);
    void setFullscreen(bool fullscreen = true);
    void setVerticalSync(bool vsync = true);

    bool update() noexcept;
    void render() noexcept;

    private:
    void terminate();

    private:
    GLFWwindow *m_window = nullptr;
    ImGuiIO m_io;
    std::string m_title{};
};
