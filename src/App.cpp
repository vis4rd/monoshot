#include "../include/App.hpp"

#include "../include/ui/fonts/Font.hpp"
#include "../include/utility/ResourceManager.hpp"

App::App(const std::string& window_title, uint32_t width, uint32_t height)
    : m_input(InputManager::get()),
      m_sectionManager(SectionManager::get())
{
    spdlog::info("App version: {}.{}.{} (build {})", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH, BUILD_NUMBER);

    if constexpr(Flag::DebugMode)  // Debug Build
    {
        m_window = std::make_shared<Window>(window_title, width, height, false, true);  // windowed, vsync
    }
    else  // Release Build
    {
        m_window = std::make_shared<Window>(window_title, width, height);  // fullscreen, vsync
    }
    ResourceManager::window = m_window;

    m_timer = std::make_shared<Timer>();
    ResourceManager::timer = m_timer;

    this->initFonts();
    this->initTextures();

    MainMenuStyle();

    m_sectionManager.emplaceSection<MainMenuSection>();
}

App::~App() noexcept
{
    this->destroyFonts();
    this->destroyTextures();
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
    res::largeTreeTexture = Resource::create<Texture::impl::Texture>("../res/textures/large_tree.png", 128, 128);
    res::smallTreeTexture = Resource::create<Texture::impl::Texture>("../res/textures/small_tree.png", 64, 64);
    res::outdoorBenchTexture = Resource::create<Texture::impl::Texture>("../res/textures/outdoors_bench.png", 48, 16);
    res::chairTexture = Resource::create<Texture::impl::Texture>("../res/textures/chair.png", 16, 16);
    res::tableTexture = Resource::create<Texture::impl::Texture>("../res/textures/table.png", 32, 32);
    res::smallBushTexture = Resource::create<Texture::impl::Texture>("../res/textures/small_bush.png", 48, 48);
    res::largeBushTexture = Resource::create<Texture::impl::Texture>("../res/textures/large_bush.png", 56, 56);
    res::carTexture = Resource::create<Texture::impl::Texture>("../res/textures/car.png", 32, 64);
    res::destroyedCarTexture = Resource::create<Texture::impl::Texture>("../res/textures/destroyed_car.png", 32, 64);

    res::rifleInventoryTexture = Resource::create<Texture::impl::Texture>("../res/textures/gun_inventory.png", 256, 128);
    res::pistolInventoryTexture = Resource::create<Texture::impl::Texture>("../res/textures/pistol_inventory.png", 256, 128);
}

void App::initFonts() noexcept
{
    using res = ResourceManager;

    const auto& window_width = res::window->getSize().x;

    // TODO: change title font size to resize dynamically or set it up just for fullscreen
    res::uiTitleFontSize = std::make_shared<float>(100.f * window_width / 1920.f);
    res::uiTitleFont = std::make_shared<Font>("../res/fonts/prisma/Prisma.ttf", *res::uiTitleFontSize);
    // res::uiButtonFontSize = std::make_shared<float>(20.f * window_width / 1920.f);
    // res::uiButtonFont = std::make_shared<Font>("../res/fonts/abandoned/Abandoned-Bold.ttf", *res::uiButtonFontSize);
    res::uiButtonFontSize = std::make_shared<float>(20.f * window_width / 1920.f);
    res::uiButtonFont = std::make_shared<Font>("../res/fonts/brass-mono/regular_comfortable.otf", *res::uiButtonFontSize);
    res::uiAmmoFontSize = std::make_shared<float>(25.f * window_width / 1920.f);
    res::uiAmmoFont = std::make_shared<Font>("../res/fonts/gunplay/GUNPLAY_.ttf", *res::uiAmmoFontSize);
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

void App::destroyTextures() noexcept
{
    using res = ResourceManager;
    res::largeTreeTexture.reset();
    res::smallTreeTexture.reset();
    res::outdoorBenchTexture.reset();
    res::chairTexture.reset();
    res::tableTexture.reset();
    res::smallBushTexture.reset();
    res::largeBushTexture.reset();
    res::carTexture.reset();
    res::destroyedCarTexture.reset();

    res::rifleInventoryTexture.reset();
    res::pistolInventoryTexture.reset();
}

void App::destroyFonts() noexcept
{
    using res = ResourceManager;

    res::uiTitleFontSize.reset();
    res::uiTitleFont.reset();
    // res::uiButtonFontSize.reset();
    // res::uiButtonFont.reset();
    res::uiButtonFontSize.reset();
    res::uiButtonFont.reset();
    res::uiAmmoFontSize.reset();
    res::uiAmmoFont.reset();
}
