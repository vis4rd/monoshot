#include "../../include/opengl/target/RenderWindow.hpp"

#include <config/StaticConfiguration.hpp>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/imgui.h>
#include <log/Logging.hpp>
#include <spdlog/spdlog.h>

namespace mono::gl
{

RenderWindow::RenderWindow(GLsizei width, GLsizei height, std::string_view title)
    : RenderTarget(width, height)
{
    spdlog::info("Creating RenderWindow '{}' with size {}x{}", title, width, height);

    this->initGlfw();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_windowHandle =
        unique_window_ptr{glfwCreateWindow(width, height, title.data(), nullptr, nullptr)};
    if(not m_windowHandle)
    {
        spdlog::critical("Failed to initialize RenderWindow native handle");
        throw std::runtime_error("Failed to initialize RenderWindow native handle");
    }

    glfwMakeContextCurrent(m_windowHandle.get());
    const auto valid_resolutions = RenderWindow::queryMonitorResolutions();
    const auto sr = valid_resolutions.front();  // smallest_resolution
    const auto lr = valid_resolutions.back();  // largest_resolution
    glfwSetWindowSizeLimits(m_windowHandle.get(), sr.x, sr.y, lr.x, lr.y);

    this->initGL();
    this->initImGui();
    this->initFlags();
}

bool RenderWindow::isFullscreen() const
{
    return m_flags[WindowFlag::FULLSCREEN];
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

void RenderWindow::setSize(GLsizei width, GLsizei height)
{
    spdlog::debug("New window size = {}x{} in pixels", width, height);
    this->RenderTarget::setSize(width, height);
    glfwSetWindowSize(m_windowHandle.get(), width, height);
}

void RenderWindow::setFullscreen(bool fullscreen)
{
    if(m_flags[WindowFlag::FULLSCREEN] == fullscreen)
    {
        // if there's no need to change anything, leave
        return;
    }

    const auto valid_resolutions = RenderWindow::queryMonitorResolutions();
    const auto sr = valid_resolutions.front();  // smallest_resolution
    const auto lr = valid_resolutions.back();  // largest_resolution
    m_flags[WindowFlag::FULLSCREEN] = fullscreen;

    // auto *current_monitor = glfwGetPrimaryMonitor();
    GLFWmonitor *current_monitor = nullptr;
    std::int32_t new_x = sr.x;
    std::int32_t new_y = sr.y;
    std::int32_t new_pos_x = (lr.x - sr.x) / 2;
    std::int32_t new_pos_y = (lr.y - sr.y) / 2;
    std::int32_t new_refresh_rate = GLFW_DONT_CARE;

    if(fullscreen)
    {
        current_monitor = glfwGetPrimaryMonitor();
        new_x = lr.x;
        new_y = lr.y;
        new_pos_x = GLFW_DONT_CARE;
        new_pos_y = GLFW_DONT_CARE;
        new_refresh_rate = RenderWindow::getRefreshRate();
    }

    glfwSetWindowMonitor(
        m_windowHandle.get(),
        current_monitor,
        new_pos_x,
        new_pos_y,
        new_x,
        new_y,
        new_refresh_rate);
    this->RenderTarget::setSize(new_x, new_y);


    m_flags[0].flip();  // override the guard of setVerticalSync() method
    this->setVerticalSync(not m_flags[0]);
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

std::string_view RenderWindow::getTitle() const
{
    // TODO(vis4rd): Implement when upgrading GLFW to 3.4
    throw std::runtime_error("Not implemented");
    // return glfwGetWindowTitle(m_windowHandle.get());
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
    std::vector<glm::ivec2> result;
    for(const auto &vm : video_modes)
    {
        result.emplace_back(vm.width, vm.height);
    }
    return result;
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

void RenderWindow::initGL() const
{
    spdlog::debug("Initializing GLAD");


    if(int success = gladLoadGL(glfwGetProcAddress); not success)
    {
        spdlog::critical("Failed to initialize GLAD");
    }

    // logging
    if constexpr(mono::config::constant::debugMode)
    {
        util::enableOpenGlLogging();
    }

    // viewport
    const auto size = m_framebuffer.getSize();
    glViewport(0, 0, size.x, size.y);
}

void RenderWindow::initImGui() const
{
    spdlog::debug("Initializing ImGui");

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    auto &io = ImGui::GetIO();
    (void)io;

    if constexpr(mono::config::constant::debugMode)
    {
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    }
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.Fonts->AddFontDefault();

    // m_io = io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(m_windowHandle.get(), true);
    ImGui_ImplOpenGL3_Init("#version 450");
}

void RenderWindow::initFlags()
{
    spdlog::debug("Initializing RenderWindow flags");

    // fullscreen
    m_flags[3] = (glfwGetWindowMonitor(m_windowHandle.get()) != nullptr);

    // maximized
    m_flags[2] = glfwGetWindowAttrib(m_windowHandle.get(), GLFW_MAXIMIZED);

    // minimized (iconified)
    m_flags[1] = glfwGetWindowAttrib(m_windowHandle.get(), GLFW_ICONIFIED);

    // vsync
    glfwSwapInterval(0);  // by default, vsync is disabled
    m_flags[0] = false;
}

}  // namespace mono::gl
