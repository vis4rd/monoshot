#include "../../include/window/Window.hpp"

Window::Window(const std::string &window_title, uint32_t width, uint32_t height) : m_title(window_title)
{
    if(!glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW.");
    }

    std::cout << "yeet" << std::endl;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_window = glfwCreateWindow(width, height, m_title.c_str(), nullptr, nullptr);
    if(!m_window)
    {
        std::cout << "Failed to create GLFW window." << std::endl;
        this->terminate();
    }

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(0);  // Disable vsync

    //
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    m_io = ImGui::GetIO();
    (void)m_io;
    m_io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    // m_io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
    m_io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;  // Enable Docking
    m_io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;  // Enable Multi-Viewport
    // m_io.ConfigViewportsNoAutoMerge = true;
    // m_io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform
    // windows can look identical to regular ones.
    ImGuiStyle &style = ImGui::GetStyle();
    if(m_io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 450");

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD." << std::endl;
        this->terminate();
    }

    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(m_window,
        [](GLFWwindow *window, int width, int height) -> void
        {
            glViewport(0, 0, width, height);
        });
    // glfwSetWindowIconifyCallback(m_window, [](GLFWwindow* window, int
    // iconified) -> void
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
    // glfwSetWindowMaximizeCallback(m_window, [](GLFWwindow* window, int
    // maximized) -> void
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

    // glfwSetWindowAspectRatio(m_window, 16, 9);
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

// bool Window::isVerticalSyncEnabled() const
// {
//     return wglSwapIntervalEXT();
//     return glXSwapIntervalEXT();
// }

void Window::setTitle(const std::string &title)
{
    m_title = title;
    glfwSetWindowTitle(m_window, m_title.c_str());
}

void Window::setFullscreen(bool fullscreen)
{
    return;
}

void Window::setVerticalSync(bool vsync)
{
    glfwSwapInterval(static_cast<int>(vsync));
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
