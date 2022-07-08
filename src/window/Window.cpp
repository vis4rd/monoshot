#include "../../include/window/Window.hpp"

Window::Window(const std::string &window_title, uint32_t width, uint32_t height)
    : m_title(window_title), m_width(width), m_height(height)
{
    if(!glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW.");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
    if(!m_window)
    {
        std::cout << "Failed to create GLFW window." << std::endl;
        this->terminate();
    }

    glfwMakeContextCurrent(m_window);
    this->setVerticalSync(true);

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

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD." << std::endl;
        this->terminate();
    }

    glViewport(0, 0, width, height);
    // glfwSetWindowSizeCallback(m_window,
    //     [](GLFWwindow *window, int new_width, int new_height) -> void
    //     {
    //         std::cout << "New window size = " << new_width << "x" << new_height << " in screen coordinates\n";
    //         // glViewport(0, 0, new_width, new_height);
    //     });

    glfwSetFramebufferSizeCallback(m_window,
        [](GLFWwindow *window, int new_width, int new_height) -> void
        {
            glViewport(0, 0, new_width, new_height);
            glfwSetWindowAspectRatio(window, 16, 9);
        });

    // glfwSetWindowContentScaleCallback(m_window,
    //     [](GLFWwindow *window, float xscale, float yscale)
    //     {
    //         // set_interface_scale(xscale, yscale);
    //     });
}

GLFWwindow *Window::getWindow()
{
    return m_window;
}

const ImGuiIO &Window::getImGuiIo() const
{
    return m_io;
}

glm::uvec2 Window::getSize() const
{
    int32_t width, height;
    glfwGetWindowSize(m_window, &width, &height);
    return glm::uvec2(width, height);
}

const std::string &Window::getTitle() const
{
    return m_title;
}

bool Window::isMaximized() const
{
    return static_cast<bool>(glfwGetWindowAttrib(m_window, GLFW_MAXIMIZED));
}

bool Window::isVerticalSyncEnabled() const
{
    std::cout << "'bool Window::isVerticalSyncEnabled() const' is not currently implemented." << std::endl;
    return false;
}

void Window::setTitle(const std::string &title)
{
    m_title = title;
    glfwSetWindowTitle(m_window, m_title.c_str());
}

void Window::setFullscreen(bool fullscreen)
{
    if(fullscreen)
    {
        glfwSetWindowMonitor(m_window, glfwGetPrimaryMonitor(), 0, 0, 1920, 1080, GLFW_DONT_CARE);
    }
    else
    {
        glfwSetWindowMonitor(m_window, nullptr, 320, 180, 1280, 720, GLFW_DONT_CARE);
    }
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
    glfwSwapInterval(static_cast<int>(vsync));
}

bool Window::update() noexcept
{
    if(glfwWindowShouldClose(m_window))
    {
        return false;
    }

    glfwPollEvents();
    return true;
}

void Window::terminate()
{
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
