#pragma once

#include <bitset>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "RenderTarget.hpp"

namespace mono::gl
{

class RenderWindow final : public RenderTarget
{
    public:
    RenderWindow(GLsizei width, GLsizei height, std::string_view title);
    RenderWindow(const RenderWindow& copy) = delete;
    RenderWindow(RenderWindow&& move) = default;
    ~RenderWindow() = default;

    RenderWindow& operator=(const RenderWindow& copy) = delete;
    RenderWindow& operator=(RenderWindow&& move) = default;

    [[nodiscard]] bool isFullscreen() const;
    [[nodiscard]] bool isMaximized() const;
    [[nodiscard]] bool isMinimized() const;
    [[nodiscard]] bool isVerticalSyncEnabled() const;

    void toggleFullscreen();

    void setSize(GLsizei width, GLsizei height);
    void setFullscreen(bool fullscreen = true);
    void setMaximized(bool maximized = true);
    void setMinimized(bool minimized = true);
    void setVerticalSync(bool vsync = true);
    void setRefreshRate(std::int32_t hz);
    void setTitle(std::string_view title);

    std::string_view getTitle() const;

    [[nodiscard]] static std::span<const GLFWvidmode> queryVideoModes();
    [[nodiscard]] static std::vector<glm::ivec2> queryMonitorResolutions();
    [[nodiscard]] static std::int32_t getRefreshRate();

    private:
    void initGlfw() const;
    void initGL() const;
    void initImGui() const;
    void initFlags();
    using RenderTarget::render;

    private:
    using unique_window_ptr = std::unique_ptr<GLFWwindow, decltype([](GLFWwindow* window) {
                                                  glfwDestroyWindow(window);
                                              })>;

    enum WindowFlag
    {
        VSYNC = 0,
        MINIMIZED = 1,
        MAXIMIZED = 2,
        FULLSCREEN = 3,
    };

    private:
    unique_window_ptr m_windowHandle = nullptr;
    /**
     * @brief Flags used to monitor window's state.
     *
     * Current flags are:
     * 3          | 2         | 1         | 0
     * fullscreen | maximized | minimized | vsync
     */
    std::bitset<4> m_flags{};
    bool m_shouldClose = false;
};

}  // namespace mono::gl
