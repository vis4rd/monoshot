#include "renderer/RenderWindow.hpp"

#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/imgui.h>
#include <spdlog/spdlog.h>

#include "mono/config/Config.hpp"
#include "mono/log/Logging.hpp"
#include "mono/util/custom_imgui/StyleColorsTransparent.hpp"

namespace mono
{

RenderWindow::RenderWindow()
    : RenderTarget()
    , m_initialWindowSize{1280, 720}
{ }

RenderWindow::RenderWindow(::gl::GLsizei width, ::gl::GLsizei height, std::string_view title)
    : RenderTarget()
    , m_initialWindowSize{width, height}
{
    this->create(width, height, title);
}

RenderWindow::~RenderWindow()
{
    this->destroyEventCallbacks();
}

void RenderWindow::activate() const
{
    ::gl::glBindFramebuffer(::gl::GL_FRAMEBUFFER, 0);
}

void RenderWindow::deactivate() const
{
    ::gl::glBindFramebuffer(::gl::GL_FRAMEBUFFER, 0);
}

void RenderWindow::create(::gl::GLsizei width, ::gl::GLsizei height, std::string_view title)
{
    spdlog::info("Creating RenderWindow '{}' with size {}x{}", title, width, height);

    this->initGlfw();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    m_windowHandle =
        window_handle_t{glfwCreateWindow(width, height, title.data(), nullptr, nullptr)};
    if(not m_windowHandle)
    {
        spdlog::critical("Failed to initialize RenderWindow native handle");
        throw std::runtime_error("Failed to initialize RenderWindow native handle");
    }

    m_userStorage = std::make_unique<gl::RenderWindowUserStorage>(*this);

    glfwMakeContextCurrent(m_windowHandle.get());
    const auto valid_resolutions = RenderWindow::queryMonitorResolutions();
    const auto sr = valid_resolutions.front();  // smallest_resolution
    const auto lr = valid_resolutions.back();  // largest_resolution
    glfwSetWindowSizeLimits(m_windowHandle.get(), sr.x, sr.y, lr.x, lr.y);

    this->initGlbinding();
    this->initGl();
    this->initImGui();
    this->initFlags();
    this->initEventCallbacks();
}

bool RenderWindow::isFullscreen() const
{
    return m_flags[WindowFlag::FULLSCREEN];
}

bool RenderWindow::isBorderlessFullscreen() const
{
    return m_flags[WindowFlag::BORDERLESS_FULLSCREEN];
}

bool RenderWindow::isMaximized() const
{
    return m_flags[WindowFlag::MAXIMIZED];
}

bool RenderWindow::isMinimized() const
{
    return m_flags[WindowFlag::MINIMIZED];
}

bool RenderWindow::isVerticalSyncEnabled() const
{
    return m_flags[WindowFlag::VSYNC];
}

void RenderWindow::toggleFullscreen()
{
    this->setFullscreen(not m_flags[WindowFlag::FULLSCREEN]);
}

void RenderWindow::toggleBorderlessFullscreen()
{
    this->setBorderlessFullscreen(not m_flags[WindowFlag::BORDERLESS_FULLSCREEN]);
}

void RenderWindow::setSize(::gl::GLsizei width, ::gl::GLsizei height)
{
    spdlog::debug("New window size = {}x{} in pixels", width, height);

    glfwSetWindowSize(m_windowHandle.get(), width, height);
}

void RenderWindow::setFullscreen(bool fullscreen)
{
    if(m_flags[WindowFlag::FULLSCREEN] == fullscreen)
    {
        // if there's no need to change anything, leave
        return;
    }

    if(m_flags[WindowFlag::MAXIMIZED])
    {
        this->setMaximized(false);
    }

    m_flags[WindowFlag::FULLSCREEN] = fullscreen;
    m_flags[WindowFlag::BORDERLESS_FULLSCREEN] = false;

    GLFWmonitor *current_monitor = nullptr;
    std::int32_t new_width = m_initialWindowSize.x;
    std::int32_t new_height = m_initialWindowSize.y;
    std::int32_t new_pos_x = 100;
    std::int32_t new_pos_y = 100;
    std::int32_t new_refresh_rate = GLFW_DONT_CARE;

    if(fullscreen)
    {
        current_monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(current_monitor);
        new_width = mode->width;
        new_height = mode->height;
        new_pos_x = GLFW_DONT_CARE;
        new_pos_y = GLFW_DONT_CARE;
        new_refresh_rate = mode->refreshRate;

        // Update window size to match the new resolution in fullscreen, so that the switch is
        // much faster. This should be set by glfwSetWindowMonitor, but for some reason it breaks on
        // multiple switching back and forth.
        glfwSetWindowSize(m_windowHandle.get(), new_width, new_height);
    }
    else
    {
        // Restore decorations in case user performed:
        // borderless fullscreen -> fullscreen -> windowed
        glfwSetWindowAttrib(m_windowHandle.get(), GLFW_DECORATED, GLFW_TRUE);
    }

    glfwSetWindowMonitor(
        m_windowHandle.get(),
        current_monitor,
        new_pos_x,
        new_pos_y,
        new_width,
        new_height,
        new_refresh_rate);

    m_flags[WindowFlag::VSYNC].flip();  // override the guard of setVerticalSync() method
    this->setVerticalSync(not m_flags[WindowFlag::VSYNC]);
}

void RenderWindow::setBorderlessFullscreen(bool borderless)
{
    if(m_flags[WindowFlag::BORDERLESS_FULLSCREEN] == borderless)
    {
        // if there's no need to change anything, leave
        return;
    }
    if(m_flags[WindowFlag::MAXIMIZED])
    {
        this->setMaximized(false);
    }
    m_flags[WindowFlag::BORDERLESS_FULLSCREEN] = borderless;
    m_flags[WindowFlag::FULLSCREEN] = false;

    std::int32_t new_width = m_initialWindowSize.x;
    std::int32_t new_height = m_initialWindowSize.y;
    std::int32_t new_pos_x = 100;
    std::int32_t new_pos_y = 100;
    std::int32_t new_refresh_rate = GLFW_DONT_CARE;
    std::int32_t new_decorated_state = GLFW_TRUE;

    if(borderless)
    {
        const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        new_width = mode->width;
        new_height = mode->height;
        new_pos_x = 0;
        new_pos_y = 0;
        new_refresh_rate = mode->refreshRate;
        new_decorated_state = GLFW_FALSE;

        // Update window size to match the new resolution in fullscreen, so that the switch is
        // much faster. This should be set by glfwSetWindowMonitor, but for some reason it breaks on
        // multiple switching back and forth.
        glfwSetWindowSize(m_windowHandle.get(), new_width, new_height);
    }

    glfwSetWindowAttrib(m_windowHandle.get(), GLFW_DECORATED, new_decorated_state);
    glfwSetWindowMonitor(
        m_windowHandle.get(),
        nullptr,
        new_pos_x,
        new_pos_y,
        new_width,
        new_height,
        new_refresh_rate);
}

void RenderWindow::setMaximized(bool maximized)
{
    if(m_flags[WindowFlag::MAXIMIZED] == maximized)
    {
        // if there's no need to change anything, leave
        return;
    }

    m_flags[WindowFlag::MAXIMIZED] = maximized;
    if(m_flags[WindowFlag::MAXIMIZED])
    {
        glfwMaximizeWindow(m_windowHandle.get());
    }
    else
    {
        glfwRestoreWindow(m_windowHandle.get());
    }
}

void RenderWindow::setMinimized(bool minimized)
{
    if(m_flags[WindowFlag::MINIMIZED] == minimized)
    {
        // if there's no need to change anything, leave
        return;
    }

    m_flags[WindowFlag::MINIMIZED] = minimized;
    if(m_flags[WindowFlag::MINIMIZED])
    {
        glfwIconifyWindow(m_windowHandle.get());
    }
    else
    {
        glfwRestoreWindow(m_windowHandle.get());
    }
}

void RenderWindow::setVerticalSync(bool vsync)
{
    if(m_flags[WindowFlag::VSYNC] == vsync)
    {
        // if there's no need to change anything, leave
        return;
    }

    m_flags[WindowFlag::VSYNC] = vsync;
    glfwSwapInterval(static_cast<int>(vsync));
}

void RenderWindow::setRefreshRate(std::int32_t hz)
{
    if(this->isFullscreen())
    {
        const auto monitor = glfwGetPrimaryMonitor();
        const auto size = this->getSize();
        glfwSetWindowMonitor(
            m_windowHandle.get(),
            monitor,
            GLFW_DONT_CARE,
            GLFW_DONT_CARE,
            size.x,
            size.y,
            hz);
        this->setVerticalSync(m_flags[WindowFlag::VSYNC]);
    }
}

void RenderWindow::setTitle(std::string_view title)
{
    glfwSetWindowTitle(m_windowHandle.get(), title.data());
}

glm::ivec2 RenderWindow::getSize() const
{
    int width{};
    int height{};
    glfwGetWindowSize(m_windowHandle.get(), &width, &height);
    return glm::ivec2{width, height};
}

std::string_view RenderWindow::getTitle() const
{
    // TODO(vis4rd): Implement when upgrading GLFW to 3.4
    throw std::runtime_error("Not implemented");
    // return glfwGetWindowTitle(m_windowHandle.get());
}

GLFWwindow *RenderWindow::getNativeWindow() const
{
    return m_windowHandle.get();
}

glm::vec2 RenderWindow::getMousePosition() const
{
    glm::dvec2 mouse_pos;
    glfwGetCursorPos(m_windowHandle.get(), &(mouse_pos.x), &(mouse_pos.y));
    return mouse_pos;
}

gl::RenderWindowUserStorage &RenderWindow::getUserStorage()
{
    return *m_userStorage;
}

void RenderWindow::prepareRender()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    this->activate();
    ::gl::glClear(::gl::GL_COLOR_BUFFER_BIT | ::gl::GL_STENCIL_BUFFER_BIT);
}

void RenderWindow::render() const
{
    if(m_shouldClose)
    {
        return;
    }

    this->prerender();

    glfwSwapBuffers(m_windowHandle.get());
}

void RenderWindow::renderCustom() const
{
    if(m_shouldClose)
    {
        return;
    }

    this->prerender();

    // Note: no usage of shader here, the user has to take care of it themselves

    glfwSwapBuffers(m_windowHandle.get());
}

void RenderWindow::requestClose()
{
    m_shouldClose = true;
}

bool RenderWindow::shouldClose()
{
    if(not m_shouldClose)
    {
        // if not set already, check if the glfw event requested to close
        m_shouldClose = glfwWindowShouldClose(m_windowHandle.get());
    }
    return m_shouldClose;
}

std::span<const GLFWvidmode> RenderWindow::queryVideoModes()
{
    auto *monitor = glfwGetPrimaryMonitor();
    if(not monitor)
    {
        return {};
    }

    int count{};
    const GLFWvidmode *modes = glfwGetVideoModes(monitor, &count);
    std::span<const GLFWvidmode> modes_span{modes, static_cast<std::size_t>(count)};
    return modes_span;
}

std::vector<glm::ivec2> RenderWindow::queryMonitorResolutions()
{
    const auto video_modes = RenderWindow::queryVideoModes();
    std::vector<glm::ivec2> retval;
    std::ranges::transform(video_modes, std::back_inserter(retval), [](const GLFWvidmode &vm) {
        return glm::ivec2{vm.width, vm.height};
    });
    return retval;
}

std::int32_t RenderWindow::getRefreshRate()
{
    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    if(not mode)
    {
        return GLFW_DONT_CARE;
    }
    return mode->refreshRate;
}

void RenderWindow::initGlfw() const
{
    spdlog::debug("Initializing GLFW");

    constexpr auto glfw_error_message_callback = [](int error, const char *description) -> void {
        spdlog::error("[GLFW ERROR {}] {}", error, description);
    };
    glfwSetErrorCallback(static_cast<GLFWerrorfun>(glfw_error_message_callback));

    if(not glfwInit())
    {
        spdlog::critical("Failed to initialize GLFW library");
        throw std::runtime_error("Failed to initialize GLFW library.");
    }
}

void RenderWindow::initGlbinding() const
{
    spdlog::debug("Initializing GLBINDING");

    glbinding::initialize(glfwGetProcAddress);
}

void RenderWindow::initGl() const
{
    spdlog::debug("Initializing GL");

    // logging
    if(const auto config_log_level =
           mono::config::runtime.get<config::OptionStringConfigItem>("engine", "LogLevel");
       config_log_level.has_value())
    {
        const spdlog::level config_level =
            config_log_level->get().getValue<spdlog::level>().value_or(spdlog::level::info);
        if(config_level <= spdlog::level::debug)
        {
            log::enableOpenGlLogging();
        }
    }
    // viewport
    const auto size = this->getSize();
    ::gl::glViewport(0, 0, size.x, size.y);
}

void RenderWindow::initImGui() const
{
    spdlog::debug("Initializing ImGui");

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    auto &io = ImGui::GetIO();
    (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.Fonts->AddFontDefault();

    // m_io = io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    mono::util::Custom::ImGui::StyleColorsTransparent();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(m_windowHandle.get(), true);
    ImGui_ImplOpenGL3_Init("#version 450");
}

void RenderWindow::initFlags()
{
    spdlog::debug("Initializing RenderWindow flags");

    // borderless fullscreen
    m_flags[WindowFlag::BORDERLESS_FULLSCREEN] =
        not glfwGetWindowAttrib(m_windowHandle.get(), GLFW_DECORATED);

    // fullscreen
    m_flags[WindowFlag::FULLSCREEN] = (glfwGetWindowMonitor(m_windowHandle.get()) != nullptr);

    // maximized
    m_flags[WindowFlag::MAXIMIZED] = glfwGetWindowAttrib(m_windowHandle.get(), GLFW_MAXIMIZED);

    // minimized (iconified)
    m_flags[WindowFlag::MINIMIZED] = glfwGetWindowAttrib(m_windowHandle.get(), GLFW_ICONIFIED);

    // vsync
    glfwSwapInterval(0);  // by default, vsync is disabled
    m_flags[WindowFlag::VSYNC] = false;
}

void RenderWindow::initEventCallbacks() const
{
    spdlog::debug("Initializing RenderWindow event callbacks");

    // update framebuffer size when user resizes the window
    glfwSetWindowSizeCallback(
        m_windowHandle.get(),
        [](GLFWwindow *window, int new_width, int new_height) -> void {
            auto &storage = mono::glfwGetWindowUserPointer(window);

            spdlog::debug("New window size = {}x{} in screen coordinates", new_width, new_height);
            storage.window.setSize(new_width, new_height);
        });

    glfwSetWindowMaximizeCallback(
        m_windowHandle.get(),
        [](GLFWwindow *window, int maximized) -> void {
            auto &storage = mono::glfwGetWindowUserPointer(window);
            auto &self = storage.window;

            self.setMaximized(static_cast<bool>(maximized));
            if(self.isMaximized())
            {
                spdlog::debug("Window has been maximized");
            }
            else
            {
                spdlog::debug("Window has been restored");
            }
        });

    glfwSetWindowIconifyCallback(
        m_windowHandle.get(),
        [](GLFWwindow *window, int minimized) -> void {
            auto &storage = mono::glfwGetWindowUserPointer(window);
            auto &self = storage.window;

            self.setMinimized(static_cast<bool>(minimized));
            if(self.isMinimized())
            {
                spdlog::debug("Window has been minimized");
            }
            else
            {
                spdlog::debug("Window has been restored");
            }
        });
}

void RenderWindow::destroyEventCallbacks() const
{
    glfwSetWindowSizeCallback(m_windowHandle.get(), nullptr);
    glfwSetWindowMaximizeCallback(m_windowHandle.get(), nullptr);
    glfwSetWindowIconifyCallback(m_windowHandle.get(), nullptr);
}

void RenderWindow::prerender() const
{
    // Render ImGui at the end of the current frame
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    const auto &io = ImGui::GetIO();
    if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow *backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

}  // namespace mono
