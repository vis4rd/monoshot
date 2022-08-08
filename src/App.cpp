#include "../include/App.hpp"

App::App(const std::string& window_title, uint32_t width, uint32_t height)
    : m_window(Window::get()),
      m_input(InputManager::get()),
      m_sectionManager(SectionManager::get())
{
    this->initLogger();
    spdlog::info("App version: {}.{}.{} (build {})", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH, BUILD_NUMBER);

    m_window.setTitle(window_title);
    m_window.setSize({width, height});

    MainMenuStyle();

    m_sectionManager.emplaceSection<MainMenuSection>();
}

App::~App() noexcept
{
    this->terminate(EXIT_SUCCESS);
}

Window& App::getWindow()
{
    return m_window;
}

const Window& App::getWindow() const
{
    return m_window;
}

void App::run() noexcept
{
    spdlog::info("Starting main application loop");
    while(m_window.update(m_sectionManager))
    {
        m_window.render(m_sectionManager);
    }
    spdlog::info("Halted main application loop");
}

static void openGLDebugMessageCallback(unsigned source, unsigned type, unsigned id, unsigned severity, int length, const char* message, const void* userParam)
{
    switch(severity)
    {
        case GL_DEBUG_SEVERITY_HIGH: spdlog::error(message); return;
        case GL_DEBUG_SEVERITY_MEDIUM: spdlog::warn(message); return;
        case GL_DEBUG_SEVERITY_LOW: spdlog::debug(message); return;
        case GL_DEBUG_SEVERITY_NOTIFICATION: spdlog::info(message); return;
    }
}

void App::initLogger() noexcept
{
    fs::create_directory("../logs");

    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::debug);  // TODO: set this to debug/info depending on build type
    console_sink->set_pattern("%^%l%$ | %v");

    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("../logs/latest.log", true);
    file_sink->set_level(spdlog::level::debug);
    file_sink->set_pattern("[%^%l%$] %v");

    spdlog::logger multisink_logger("logger", {console_sink, file_sink});
    multisink_logger.set_level(spdlog::level::debug);
    spdlog::set_default_logger(std::make_shared<spdlog::logger>(multisink_logger));

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(openGLDebugMessageCallback, nullptr);

    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);

    spdlog::debug("Logging initialized");
}

void App::terminate(int code) noexcept
{
    spdlog::info("Closing the application");
    std::exit(code);
}
