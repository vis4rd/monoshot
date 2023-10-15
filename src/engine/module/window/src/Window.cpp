#include "../include/window/Window.hpp"

#include "resource/ResourceManager.hpp"

namespace mono
{

Window::Window()
    : Window("MONOSHOT", 1280, 720, false, true)
{ }

Window::Window(
    const std::string &title,
    std::uint32_t width,
    std::uint32_t height,
    bool fullscreen,
    bool vsync)
    : NativeWindow(title, width, height)
    , m_screenVa()
    , m_screenFb(m_width, m_height)
{
    spdlog::info("Creating window instance");

    this->initImGui();

    this->setFullscreen(fullscreen);
    this->setVerticalSync(vsync);
    m_isMaximized = static_cast<bool>(glfwGetWindowAttrib(m_window, GLFW_MAXIMIZED));

    constexpr std::array<float, 16> screenVertexBuffer =
        {-1.f, -1.f, 0.f, 0.f, 1.f, -1.f, 1.f, 0.f, 1.f, 1.f, 1.f, 1.f, -1.f, 1.f, 0.f, 1.f};
    constexpr std::array<std::uint32_t, 6> screenElementBuffer = {0, 1, 2, 2, 3, 0};

    VertexBuffer screen_vb(screenVertexBuffer.data(), screenVertexBuffer.size() * sizeof(float));
    BufferLayout layout = {
        {ShaderDataType::FLOAT2, "aPos"      },
        {ShaderDataType::FLOAT2, "aTexCoords"},
    };
    screen_vb.setLayout(layout);

    ElementBuffer screen_eb(screenElementBuffer.data(), 6);
    m_screenVa.addVertexBuffer(std::move(screen_vb));
    m_screenVa.addElementBuffer(screen_eb);

    mono::gl::ShaderManager::get().addShaderProgram(
        "screen",
        "../res/shaders/screen.vert",
        "../res/shaders/screen.frag");

    // update internal size tracking for UI and framebuffer size when user resizes the window
    glfwSetWindowSizeCallback(
        m_window,
        [](GLFWwindow *window, int new_width, int new_height) -> void {
            spdlog::debug("New window size = {}x{} in screen coordinates", new_width, new_height);
            auto &self = ResourceManager::window;
            self->m_width = new_width;
            self->m_height = new_height;
            self->setFramebufferSize({new_width, new_height});
        });

    glfwSetWindowMaximizeCallback(m_window, [](GLFWwindow *window, int maximized) -> void {
        auto &self = ResourceManager::window;
        self->m_isMaximized = static_cast<bool>(maximized);
        if(self->m_isMaximized)
        {
            spdlog::debug("Window has been maximized");
        }
        else
        {
            spdlog::debug("Window has been restored");
        }
    });

    glfwSetWindowIconifyCallback(m_window, [](GLFWwindow *window, int minimized) -> void {
        auto &self = ResourceManager::window;
        self->m_isMinimized = static_cast<bool>(minimized);
        if(self->m_isMinimized)
        {
            spdlog::debug("Window has been minimized");
        }
        else
        {
            spdlog::debug("Window has been restored");
        }
    });

    glm::vec4 clear_color = glm::vec4(0.f, 0.f, 0.f, 1.f);
    glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
}

Window::~Window()
{
    spdlog::info("Terminating window instance");

    m_screenFb.unbind();
    m_screenVa.unbind();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

const ImGuiIO &Window::getImGuiIo() const
{
    return m_io;
}

FrameBuffer &Window::getFramebuffer()
{
    return m_screenFb;
}

glm::vec2 Window::getMousePosition() const
{
    glm::dvec2 mouse_pos;
    glfwGetCursorPos(m_window, &(mouse_pos.x), &(mouse_pos.y));
    return mouse_pos;
}

bool Window::isFullscreen() const
{
    return m_isFullscreen;
}

bool Window::isMaximized() const
{
    return m_isMaximized;
}

bool Window::isVerticalSyncEnabled() const
{
    return m_isVSyncEnabled;
}

void Window::toggleFullscreen()
{
    this->setFullscreen(!m_isFullscreen);
}

void Window::setSize(const std::pair<int32_t, int32_t> &new_size)
{
    spdlog::debug("New window size = {}x{} in pixels", new_size.first, new_size.second);
    NativeWindow::setSize(new_size);
}

void Window::setFramebufferSize(const std::pair<int32_t, int32_t> &new_size)
{
    spdlog::debug("New framebuffer size = {}x{} in pixels", new_size.first, new_size.second);
    m_screenFb.resize(new_size.first, new_size.second);
}

void Window::setFullscreen(bool fullscreen)
{
    if(m_isFullscreen == fullscreen)  // if there's no need to change anything, leave
    {
        return;
    }

    const auto hz = NativeWindow::getRefreshRate();
    const auto valid_resolutions = NativeWindow::queryMonitorResolutions();
    const auto monitor = NativeWindow::getCurrentMonitor();
    const auto sr = valid_resolutions.front();  // smallest_resolution
    const auto lr = valid_resolutions.back();  // largest_resolution
    m_isFullscreen = fullscreen;
    if(fullscreen)
    {
        glfwSetWindowMonitor(m_window, monitor, GLFW_DONT_CARE, GLFW_DONT_CARE, lr.x, lr.y, hz);
        this->setFramebufferSize({lr.x, lr.y});
        m_width = lr.x;
        m_height = lr.y;
    }
    else
    {
        glfwSetWindowMonitor(
            m_window,
            nullptr,
            (lr.x - sr.x) / 2.f,
            (lr.y - sr.y) / 2.f,
            sr.x,
            sr.y,
            GLFW_DONT_CARE);
        this->setFramebufferSize({sr.x, sr.y});  // update framebuffer size when leaving fullscreen
        m_width = sr.x;  // update internal size indicator
        m_height = sr.y;
    }

    m_isVSyncEnabled = !m_isVSyncEnabled;  // override the guard of setVerticalSync() method
    this->setVerticalSync(!m_isVSyncEnabled);
}

void Window::setMaximized(bool maximized)
{
    if(m_isMaximized == maximized)  // if there's no need to change anything, leave
    {
        return;
    }

    m_isMaximized = maximized;
    if(m_isMaximized)
    {
        glfwMaximizeWindow(m_window);
    }
    else
    {
        glfwRestoreWindow(m_window);
    }
}

void Window::setMinimized(bool minimized)
{
    if(m_isMinimized == minimized)  // if there's no need to change anything, leave
    {
        return;
    }

    m_isMinimized = minimized;
    if(m_isMinimized)
    {
        glfwIconifyWindow(m_window);
    }
    else
    {
        glfwRestoreWindow(m_window);
    }
}

void Window::setVerticalSync(bool vsync)
{
    if(m_isVSyncEnabled == vsync)  // if there's no need to change anything, leave
    {
        return;
    }

    m_isVSyncEnabled = vsync;
    glfwSwapInterval(static_cast<int>(vsync));
}

void Window::setRefreshRate(std::uint32_t hz)
{
    if(this->isFullscreen())
    {
        const auto monitor = NativeWindow::getCurrentMonitor();
        glfwSetWindowMonitor(
            m_window,
            monitor,
            GLFW_DONT_CARE,
            GLFW_DONT_CARE,
            m_width,
            m_height,
            hz);
        this->setVerticalSync(m_isVSyncEnabled);
    }
}

void Window::initImGui()
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

    m_io = io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 450");
}

}  // namespace mono
