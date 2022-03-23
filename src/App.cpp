#include "../include/App.hpp"

App::App(const std::string& window_title, uint32_t width, uint32_t height)
{
    if(!glfwInit()) { throw std::runtime_error("Failed to initialize GLFW."); }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(width, height, window_title.c_str(), nullptr, nullptr);
    if (!m_window)
    {
        std::cout << "Failed to create GLFW window." << std::endl;
        this->terminate(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(m_window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD." << std::endl;
        this->terminate(EXIT_FAILURE);
    }

    glViewport(0, 0, width, height);
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
    glfwTerminate();
    std::exit(code);
}
