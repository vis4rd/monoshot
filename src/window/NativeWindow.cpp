#include "../../include/window/NativeWindow.hpp"

#include <spdlog/spdlog.h>
#include <glad/glad.h>

#include "../../include/utility/Logging.hpp"

NativeWindow::NativeWindow(const std::string &title, std::int32_t width, std::int32_t height)
    : m_title(title),
      m_width(width),
      m_height(height)
{
    this->initGLFW();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
    if(!m_window)
    {
        spdlog::critical("Failed to initialize native window");
        throw std::runtime_error("Failed to initialize native window");
    }
    glfwMakeContextCurrent(m_window);

    const auto valid_resolutions = NativeWindow::queryMonitorResolutions();
    const auto sr = valid_resolutions.front();  // smallest_resolution
    const auto lr = valid_resolutions.back();  // largest_resolution
    glfwSetWindowSizeLimits(m_window, sr.x, sr.y, lr.x, lr.y);

    this->initGL();
}

NativeWindow::~NativeWindow()
{
    if(m_window != nullptr)
    {
        spdlog::debug("Destroying native window");
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }

    glfwTerminate();
}

GLFWwindow *NativeWindow::getNativeWindow()
{
    return m_window;
}

const std::string &NativeWindow::getTitle() const
{
    return m_title;
}

glm::ivec2 NativeWindow::getSize() const
{
    return {m_width, m_height};
}

void NativeWindow::setSize(const std::pair<int32_t, int32_t> &new_size)
{
    m_width = new_size.first;
    m_height = new_size.second;
    glfwSetWindowSize(m_window, m_width, m_height);
}

void NativeWindow::setTitle(const std::string &title)
{
    m_title = title;
    glfwSetWindowTitle(m_window, m_title.c_str());
}

GLFWmonitor *NativeWindow::getCurrentMonitor()
{
    return glfwGetPrimaryMonitor();
}

std::pair<const GLFWvidmode *, int> NativeWindow::getVideoModes()
{
    auto monitor = NativeWindow::getCurrentMonitor();
    if(!monitor)
    {
        return std::make_pair(nullptr, 0);
    }
    int count;
    const GLFWvidmode *modes = glfwGetVideoModes(monitor, &count);
    return std::make_pair(modes, count);
}

std::int32_t NativeWindow::getRefreshRate()
{
    const GLFWvidmode *mode = glfwGetVideoMode(NativeWindow::getCurrentMonitor());
    if(!mode)
    {
        return 0;
    }
    return mode->refreshRate;
}

std::vector<glm::ivec2> NativeWindow::queryMonitorResolutions()
{
    const auto &[video_modes, vm_count] = NativeWindow::getVideoModes();
    const auto hz = NativeWindow::getRefreshRate();
    spdlog::debug("Monitor refresh rate: {}Hz", hz);
    std::vector<glm::ivec2> result;
    for(int i = 0; i < vm_count; i++)
    {
        const auto &vm = video_modes[i];
        if((vm.width % 16 == 0) && (vm.height % 9 == 0) && (vm.refreshRate == hz))
        {
            spdlog::debug("Found video mode: r{}g{}b{}, {}x{}, {}Hz", vm.redBits, vm.greenBits, vm.blueBits, vm.width, vm.height, vm.refreshRate);
            result.emplace_back(vm.width, vm.height);
        }
    }
    return result;
}

std::vector<std::uint32_t> NativeWindow::queryMonitorRefreshRates()
{
    const auto &[video_modes, vm_count] = NativeWindow::getVideoModes();
    std::vector<std::uint32_t> result;
    const auto is_already_added = [&result](const auto hz) -> bool
    {
        return result.end() != std::find(result.begin(), result.end(), hz);
    };

    for(int i = 0; i < vm_count; i++)
    {
        const auto &vm = video_modes[i];
        spdlog::debug("Found refresh rate: {}Hz", vm.refreshRate);
        if(!is_already_added(vm.refreshRate))
        {
            result.emplace_back(vm.refreshRate);
        }
    }
    std::sort(result.begin(), result.end());
    return result;
}

static void glfwErrorMessageCallback(int error_code, const char *description)
{
    spdlog::error("[GLFW ERROR {}] {}", error_code, description);
}

void NativeWindow::initGLFW()
{
    spdlog::debug("Initializing GLFW");
    glfwSetErrorCallback((GLFWerrorfun)glfwErrorMessageCallback);

    if(!glfwInit())
    {
        spdlog::critical("Failed to initialize GLFW library");
        throw std::runtime_error("Failed to initialize GLFW library.");
    }
}

void NativeWindow::initGL()
{
    spdlog::debug("Initializing GLAD");

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        spdlog::critical("Failed to initialize GLAD");
    }

    // logging
    util::enableOpenGlLogging();

    // viewport
    glViewport(0, 0, m_width, m_height);
}
