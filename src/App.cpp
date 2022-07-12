#include "../include/App.hpp"

App::App(const std::string& window_title, uint32_t width, uint32_t height)
    : m_window(window_title, width, height), m_input(Input::get()), m_sectionManager(SectionManager::get())
{
    this->initLogger();
    spdlog::info("App version: {}.{}.{} (build {})", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH, BUILD_NUMBER);

    auto key_group = m_input.addGroup("main");
    m_input.addKeybind(
        "main",
        GLFW_KEY_ESCAPE,
        GLFW_PRESS,
        [](GLFWwindow* window)
        {
            glfwSetWindowShouldClose(window, true);
        },
        m_window.getWindow());

    m_input.addKeybind("main",
        GLFW_KEY_ENTER,
        GLFW_PRESS,
        []()
        {
            std::cout << "Hooray!\n";
        });

    MainMenuStyle();

    m_sectionManager.emplaceSection<DebugSection>();
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
    while(m_window.update())
    {
        m_input.processGroup(m_window.getWindow(), "main");
        m_window.render(m_sectionManager);
    }
}

void App::initLogger() noexcept
{
    fs::create_directory("../logs");

    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::trace);  // set this to debug/info depending on build type
    console_sink->set_pattern("%^%l%$ | %v");

    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("../logs/latest.log", true);
    file_sink->set_level(spdlog::level::trace);
    file_sink->set_pattern("[%^%l%$] %v");

    spdlog::logger multisink_logger("logger", {console_sink, file_sink});
    multisink_logger.set_level(spdlog::level::trace);
    spdlog::set_default_logger(std::make_shared<spdlog::logger>(multisink_logger));

    spdlog::debug("Logging initialized");
}

void App::terminate(int code) noexcept
{
    std::exit(code);
}
