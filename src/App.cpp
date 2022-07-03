#include "../include/App.hpp"

App::App(const std::string& window_title, uint32_t width, uint32_t height)
{
    if(!glfwInit()) { throw std::runtime_error("Failed to initialize GLFW."); }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    m_window = glfwCreateWindow(width, height, window_title.c_str(), nullptr, nullptr);
    if (!m_window)
    {
        std::cout << "Failed to create GLFW window." << std::endl;
        this->terminate(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(m_window);
    // glfwSwapInterval(1); // Enable vsync

    //
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    m_io = ImGui::GetIO(); (void)m_io;
    m_io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //m_io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    m_io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    m_io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //m_io.ConfigViewportsNoAutoMerge = true;
    m_io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (m_io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    const char* glsl_version = "#version 450";
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    ///

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD." << std::endl;
        this->terminate(EXIT_FAILURE);
    }

    glViewport(0, 0, width/2, height/2);
    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) -> void
    {
        glViewport(0, 0, width, height);
    });
    // glfwSetWindowIconifyCallback(m_window, [](GLFWwindow* window, int iconified) -> void
    // {
    //     if(iconified)
    //     {
    //         glfwIconifyWindow(window);
    //     }
    //     else
    //     {
    //         glfwRestoreWindow(window);
    //     }
    // });
    // glfwSetWindowMaximizeCallback(m_window, [](GLFWwindow* window, int maximized) -> void
    // {
    //     if(maximized)
    //     {
    //         glfwMaximizeWindow(window);
    //     }
    //     else
    //     {
    //         glfwRestoreWindow(window);
    //     }
    // });

    glfwSetWindowSizeLimits(m_window, 640, 360, GLFW_DONT_CARE, GLFW_DONT_CARE);
    glfwSetWindowAspectRatio(m_window, 16, 9);
}

App::~App() noexcept
{
    this->terminate(EXIT_SUCCESS);
}

GLFWwindow* App::getWindow()
{
    return m_window;
}

const ImGuiIO& App::getImGuiIo() const
{
    return m_io;
}

void App::setWindowTitle(const std::string& title)
{
    glfwSetWindowTitle(m_window, title.c_str());
}

void App::setFullscreen(const bool fullscreen)
{
    auto* monitor = glfwGetPrimaryMonitor();
    const auto* mode = glfwGetVideoMode(monitor);
    if(fullscreen)
    {
        glfwSetWindowMonitor(
            m_window,
            monitor,
            0,
            0,
            mode->width,
            mode->height,
            mode->refreshRate);
    }
    else
    {
        glfwSetWindowMonitor(
            m_window,
            nullptr,
            mode->width - (1280/2),
            mode->height - (720/2),
            1280,
            720,
            0);
    }
}

void App::terminate(int code) noexcept
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_window);
    glfwTerminate();

    std::exit(code);
}
