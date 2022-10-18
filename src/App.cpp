#include "../include/App.hpp"
#include "../include/utility/ResourceManager.hpp"

App::App(const std::string& window_title, uint32_t width, uint32_t height)
    : m_input(InputManager::get()),
      m_sectionManager(SectionManager::get())
{
    spdlog::info("App version: {}.{}.{} (build {})", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH, BUILD_NUMBER);

    m_window = std::make_shared<Window>(window_title, width, height, true, true);
    ResourceManager::window = m_window;

    m_timer = std::make_shared<Timer>();
    ResourceManager::timer = m_timer;

    this->initTextures();

    MainMenuStyle();

    m_sectionManager.emplaceSection<MainMenuSection>();
}

App::~App() noexcept
{
    this->terminate(EXIT_SUCCESS);
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

    spdlog::debug("Logging initialized");
}

void App::initTextures() noexcept
{
    using res = ResourceManager;
    res::largeTreeTexture = std::make_shared<Texture2D>(128, 128);
    res::largeTreeTexture->load("../res/textures/small_tree.png");
}

Window& App::getWindow()
{
    return *m_window;
}

const Window& App::getWindow() const
{
    return *m_window;
}

void App::run() noexcept
{
    spdlog::info("Starting main application loop");
    while(m_window->update(m_sectionManager))
    {
        m_timer->update();
        m_window->render(m_sectionManager);
    }
    spdlog::info("Halted main application loop");
}

void App::terminate(int code) noexcept
{
    spdlog::info("Closing the application");
    std::exit(code);
}
