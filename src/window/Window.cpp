#include "../../include/window/Window.hpp"

#include "../../include/utility/ResourceManager.hpp"

Window::Window()
    : Window("Arcade Game", 1280, 720, false)
{
}

Window::Window(const std::string &title, std::uint32_t width, std::uint32_t height, bool fullscreen, bool vsync)
    : NativeWindow(title, width, height),
      screenVA(),
      screenFB(m_width, m_height)
{
    spdlog::info("Creating window instance");

    this->initImGui();

    this->setFullscreen(fullscreen);
    this->setVerticalSync(vsync);

    constexpr float screen_vertex_buffer[16] = {-1.f, -1.f, 0.f, 0.f, 1.f, -1.f, 1.f, 0.f, 1.f, 1.f, 1.f, 1.f, -1.f, 1.f, 0.f, 1.f};
    constexpr std::uint32_t screen_element_buffer[6] = {0, 1, 2, 2, 3, 0};

    VertexBuffer screenVB(screen_vertex_buffer, sizeof(screen_vertex_buffer));
    BufferLayout layout({
        BufferElement(ShaderDataType::float2, "aPos"),
        BufferElement(ShaderDataType::float2, "aTexCoords"),
    });
    screenVB.setLayout(layout);

    ElementBuffer screenEB(screen_element_buffer, 6);
    screenVA.addVertexBuffer(std::move(screenVB));
    screenVA.addElementBuffer(screenEB);

    ShaderManager::addShaderProgram("../res/shaders", "screen");

    // update internal size tracking for UI and framebuffer size when user resizes the window
    glfwSetWindowSizeCallback(m_window,
        [](GLFWwindow *window, int new_width, int new_height) -> void
        {
            spdlog::debug("New window size = {}x{} in screen coordinates", new_width, new_height);
            auto &_this = ResourceManager::window;
            _this->m_width = new_width;
            _this->m_height = new_height;
            _this->setFramebufferSize({new_width, new_height});
        });

    glm::vec4 clear_color = glm::vec4(0.f, 0.f, 0.f, 1.f);
    glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
}

Window::~Window()
{
    spdlog::info("Terminating window instance");

    screenFB.unbind();
    screenVA.unbind();

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
    return screenFB;
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
    return static_cast<bool>(glfwGetWindowAttrib(m_window, GLFW_MAXIMIZED));
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
    screenFB.resize(new_size.first, new_size.second);
}

void Window::setFullscreen(bool fullscreen)
{
    const auto hz = NativeWindow::getRefreshRate();
    const auto valid_resolutions = NativeWindow::queryMonitorResolutions();
    const auto monitor = NativeWindow::getCurrentMonitor();
    const auto sr = valid_resolutions.front();  // smallest_resolution
    const auto lr = valid_resolutions.back();  // largest_resolution
    m_isFullscreen = fullscreen;
    if(fullscreen)
    {
        glfwSetWindowMonitor(m_window, monitor, GLFW_DONT_CARE, GLFW_DONT_CARE, lr.x, lr.y, hz);
    }
    else
    {
        glfwSetWindowMonitor(m_window, nullptr, (lr.x - sr.x) / 2.f, (lr.y - sr.y) / 2.f, sr.x, sr.y, GLFW_DONT_CARE);
        this->setFramebufferSize({sr.x, sr.y});  // update framebuffer size when leaving fullscreen
        m_width = sr.x;  // update internal size indicator
        m_height = sr.y;
    }
    this->setVerticalSync(m_isVSyncEnabled);
}

void Window::setMaximized(bool maximized)
{
    if(maximized)
    {
        glfwMaximizeWindow(m_window);
    }
    else
    {
        glfwRestoreWindow(m_window);
    }
}

void Window::setVerticalSync(bool vsync)
{
    m_isVSyncEnabled = vsync;
    glfwSwapInterval(static_cast<int>(vsync));
}

void Window::setRefreshRate(std::uint32_t hz)
{
    if(this->isFullscreen())
    {
        const auto monitor = NativeWindow::getCurrentMonitor();
        glfwSetWindowMonitor(m_window, monitor, GLFW_DONT_CARE, GLFW_DONT_CARE, m_width, m_height, hz);
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

    // TODO: disable this functionality in release builds
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;  // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;  // Enable Multi-Viewport

    io.Fonts->AddFontDefault();

    m_io = io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 450");
}
