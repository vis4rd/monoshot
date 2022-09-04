#include "../../include/window/Window.hpp"

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
    this->initKeybinds();

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

    // glfwSetWindowSizeCallback(m_window,
    //     [](GLFWwindow *window, int new_width, int new_height) -> void
    //     {
    //         spdlog::debug("New window size = {}x{} in screen coordinates", new_width, new_height);
    //         glViewport(0, 0, new_width, new_height);
    //     });

    glfwSetWindowUserPointer(m_window, static_cast<void *>(this));
    glfwSetFramebufferSizeCallback(m_window,
        [](GLFWwindow *window, int new_width, int new_height) -> void
        {
            auto _this = static_cast<Window *>(glfwGetWindowUserPointer(window));
            spdlog::debug("New framebuffer size = {}x{} in pixels", new_width, new_height);
            _this->setSize({new_width, new_height});
        });

    // glfwSetWindowContentScaleCallback(m_window,
    //     [](GLFWwindow *window, float xscale, float yscale)
    //     {
    //         // set_interface_scale(xscale, yscale);
    //     });
}

Window::~Window()
{
    spdlog::info("Terminating window instance");

    glfwSetWindowUserPointer(m_window, nullptr);

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
    NativeWindow::setSize(new_size);
    // screenFB.resize(m_width, m_height);
}

void Window::setFramebufferSize(const std::pair<int32_t, int32_t> &new_size)
{
    screenFB.resize(m_width, m_height);
}

void Window::setFullscreen(bool fullscreen)
{
    const auto hz = NativeWindow::getRefreshRate();
    const auto valid_resolutions = NativeWindow::queryMonitorResolutions();
    const auto monitor = NativeWindow::getCurrentMonitor();
    const auto video_mode = glfwGetVideoMode(monitor);
    const auto sr = valid_resolutions.front();  // smallest_resolution
    const auto lr = valid_resolutions.back();  // largest_resolution
    m_isFullscreen = fullscreen;
    if(fullscreen)
    {
        glfwSetWindowMonitor(m_window, monitor, 0, 0, video_mode->width, video_mode->height, hz);
    }
    else
    {
        glfwSetWindowMonitor(m_window, nullptr, (lr.x - sr.x) / 2.f, (lr.y - sr.y) / 2.f, sr.x, sr.y, hz);
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

void Window::initImGui()
{
    spdlog::debug("Initializing ImGui");

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    auto &io = ImGui::GetIO();
    (void)io;
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;  // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;  // Enable Multi-Viewport
    // io.ConfigViewportsNoAutoMerge = true;
    // io.ConfigViewportsNoTaskBarIcon = true;
    m_io = io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 450");
}

void Window::initKeybinds()
{
    auto id = m_inputManager.addGroup("window");
    m_inputManager.addKeybind("window",
        GLFW_KEY_F11,
        KeyState::PRESS_ONCE,
        [this]
        {
            this->toggleFullscreen();
        });
    m_inputManager.addKeybind("window",
        GLFW_KEY_ESCAPE,
        KeyState::PRESS_ONCE,
        [&should_close = m_shouldClose]
        {
            if(SectionManager::get().size() == 1)
            {
                should_close = true;
            }
        });
}
