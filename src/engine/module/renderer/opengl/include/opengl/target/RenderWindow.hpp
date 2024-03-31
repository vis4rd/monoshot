#pragma once

#include <bitset>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "../glfw/RenderWindowUserStorage.hpp"
#include "RenderTarget.hpp"

namespace mono::gl
{

class RenderWindow final : public RenderTarget
{
    public:
    /**
     * @brief Construct empty RenderWindow.
     *
     * This constructor does not create any resources. Call `create()` to properly initialize the
     * object.
     */
    RenderWindow();
    RenderWindow(GLsizei width, GLsizei height, std::string_view title);
    RenderWindow(const RenderWindow& copy) = delete;
    RenderWindow(RenderWindow&& move) = default;
    ~RenderWindow();

    RenderWindow& operator=(const RenderWindow& copy) = delete;
    RenderWindow& operator=(RenderWindow&& move) = default;

    void create(GLsizei width, GLsizei height, std::string_view title);

    [[nodiscard]] bool isFullscreen() const;
    [[nodiscard]] bool isMaximized() const;
    [[nodiscard]] bool isMinimized() const;
    [[nodiscard]] bool isVerticalSyncEnabled() const;

    void toggleFullscreen();

    void setSize(GLsizei width, GLsizei height) override;
    void setFullscreen(bool fullscreen = true);
    void setMaximized(bool maximized = true);
    void setMinimized(bool minimized = true);
    void setVerticalSync(bool vsync = true);
    void setRefreshRate(std::int32_t hz);
    void setTitle(std::string_view title);

    [[nodiscard]] std::string_view getTitle() const;
    [[nodiscard]] GLFWwindow* getNativeWindow() const;
    [[nodiscard]] glm::vec2 getMousePosition() const;
    [[nodiscard]] RenderWindowUserStorage& getUserStorage();

    /**
     * @brief Prepare next frame for rendering.
     *
     * @remark This function can be called multiple times before calling `render()`.
     *
     * @note This function does NOT render anything to the screen. Use `render()` or
     *       `renderCustom()` to submit geometry.
     */
    void prepareRender();

    /**
     * @brief Render the geometry to the screen.
     *
     * This function uses a default shader program, which is a simple passthrough.
     * If you want to use a custom shader program (i.e. for post-processing), use `renderCustom()`
     * instead.
     *
     * @attention This function should be called after `prepareRender()`.
     */
    void render() const override final;

    /**
     * @brief Render the geometry to the screen using user-defined custom shader.
     *
     * @note The user is responsible for calling `glUseProgram` with the desired shader program
     *       before calling this function.
     *
     * @attention Fragment shader in the specified shader program must declare `sampler2D` uniform
     *            variable at location `0` to be bound at unit `0`.
     *
     * @attention This function should be called after `prepareRender()`.
     */
    void renderCustom() const;

    /**
     * @brief Ask the RenderWindow to terminate gracefully.
     *
     * This function sets the flag to close the window. When the flag is set, render function does
     * not do anyting. The application should check the status of the window every frame using
     * `shouldClose()` and leave the event loop when necessary.
     */
    void requestClose();

    /**
     * @brief Check if the RenderWindow wants to terminate.
     * @return True if either `requestClose()` was called or user clicked the close button manually,
     *         false otherwise.
     */
    [[nodiscard]] bool shouldClose();

    [[nodiscard]] static std::span<const GLFWvidmode> queryVideoModes();
    [[nodiscard]] static std::vector<glm::ivec2> queryMonitorResolutions();
    [[nodiscard]] static std::int32_t getRefreshRate();

    private:
    void initGlfw() const;
    void initGlad() const;
    void initGl() const;
    void initImGui() const;
    void initFlags();
    void initUserStorage();
    void initEventCallbacks() const;
    void destroyUserStorage();
    void destroyEventCallbacks() const;
    void prerender() const;

    // hide some methods from the base class
    using RenderTarget::create;
    using RenderTarget::activate;
    using RenderTarget::deactivate;

    private:
    enum WindowFlag
    {
        VSYNC = 0,
        MINIMIZED = 1,
        MAXIMIZED = 2,
        FULLSCREEN = 3,
    };

    struct GlfwWindowDestructor
    {
        void operator()(GLFWwindow* ptr) { glfwDestroyWindow(ptr); }
    };

    using window_handle_t = std::unique_ptr<GLFWwindow, GlfwWindowDestructor>;

    private:
    window_handle_t m_windowHandle = nullptr;
    /**
     * @brief Flags used to monitor window's state.
     *
     * Current flags are:
     * 3          | 2         | 1         | 0
     * fullscreen | maximized | minimized | vsync
     */
    std::bitset<4> m_flags{};
    bool m_shouldClose = false;
    RenderWindowUserStorage m_userStorage{.window = *this};
};

}  // namespace mono::gl
