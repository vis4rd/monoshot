#include "../../include/window/Window.hpp"

Window &Window::get()
{
    static Window instance;
    return instance;
}

Window::Window()
{
    spdlog::info("Creating window instance");

    this->initGLFW();
    this->initImGui();
    this->initGL();
    this->initKeybinds();

    glEnable(GL_MULTISAMPLE);

    // glfwSetWindowSizeCallback(m_window,
    //     [](GLFWwindow *window, int new_width, int new_height) -> void
    //     {
    //         spdlog::debug("New window size = {}x{} in screen coordinates", new_width, new_height);
    //         glViewport(0, 0, new_width, new_height);
    //     });

    glfwSetFramebufferSizeCallback(m_window,
        [](GLFWwindow *window, int new_width, int new_height) -> void
        {
            spdlog::debug("New framebuffer size = {}x{} in pixels", new_width, new_height);
            glViewport(0, 0, new_width, new_height);
            glfwSetWindowAspectRatio(window, 16, 9);
        });

    // glfwSetWindowContentScaleCallback(m_window,
    //     [](GLFWwindow *window, float xscale, float yscale)
    //     {
    //         // set_interface_scale(xscale, yscale);
    //     });
}

GLFWwindow *Window::getGlfwWindow()
{
    return m_window;
}

const ImGuiIO &Window::getImGuiIo() const
{
    return m_io;
}

std::pair<int32_t, int32_t> Window::getSize() const
{
    int32_t width, height;
    glfwGetWindowSize(m_window, &width, &height);
    return {width, height};
}

const std::string &Window::getTitle() const
{
    return m_title;
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
    m_width = new_size.first;
    m_height = new_size.second;
    glfwSetWindowSize(m_window, m_width, m_height);
}

void Window::setTitle(const std::string &title)
{
    m_title = title;
    glfwSetWindowTitle(m_window, m_title.c_str());
}

void Window::setFullscreen(bool fullscreen)
{
    m_isFullscreen = fullscreen;
    if(fullscreen)
    {
        glfwSetWindowMonitor(m_window, glfwGetPrimaryMonitor(), 0, 0, 1920, 1080, GLFW_DONT_CARE);
    }
    else
    {
        glfwSetWindowMonitor(m_window, nullptr, m_width / 4.f, m_height / 4.f, 1280, 720, GLFW_DONT_CARE);
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

void Window::terminate()
{
    spdlog::info("Terminating window instance");

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    if(m_window != nullptr)
    {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
    glfwTerminate();
}

void Window::initGLFW()
{
    spdlog::debug("Initializing GLFW");

    if(!glfwInit())
    {
        spdlog::critical("Failed to initialize GLFW library");
        throw std::runtime_error("Failed to initialize GLFW library.");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 16);

    m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
    if(!m_window)
    {
        spdlog::critical("Failed to initialize GLFW window");
        this->terminate();
    }

    glfwMakeContextCurrent(m_window);
    this->setVerticalSync(true);
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

static void openGLDebugMessageCallback(unsigned source, unsigned type, unsigned id, unsigned severity, int length, const char *message, const void *userParam)
{
    switch(severity)
    {
        case GL_DEBUG_SEVERITY_HIGH: spdlog::error(message); return;
        case GL_DEBUG_SEVERITY_MEDIUM: spdlog::warn(message); return;
        case GL_DEBUG_SEVERITY_LOW: spdlog::debug(message); return;
        case GL_DEBUG_SEVERITY_NOTIFICATION: spdlog::info(message); return;
    }
}

void Window::initGL()
{
    spdlog::debug("Initializing GLAD");

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        spdlog::critical("Failed to initialize GLAD");
        this->terminate();
    }

    // logging
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(openGLDebugMessageCallback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);

    // viewport
    glViewport(0, 0, m_width, m_height);
}

void Window::initKeybinds()
{
    auto id = m_inputManager.addGroup("window");
    m_inputManager.addKeybind("window",
        GLFW_KEY_F11,
        KeyState::PRESS_ONCE,
        []
        {
            auto &window = Window::get();
            window.toggleFullscreen();
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
