#include "../include/App.hpp"

#include <filesystem>

#include <cstring/cstring.hpp>
#include <opengl/texture/Texture.hpp>
#include <resource/Resource.hpp>
#include <resource/ResourceManager.hpp>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <ui/Font.hpp>

#include "../include/section/MainMenuSection.hpp"

App::App(const std::string& window_title, uint32_t width, uint32_t height)
    : m_configLoader(mono::ConfigLoader::get())
    , m_input(InputManager::get())
    , m_sectionManager(SectionManager::get())
{
    spdlog::info("App version: {}", MONOSHOT_VERSION);

    if constexpr(mono::config::constant::debugMode)  // Debug Build
    {
        // windowed, vsync
        m_window = std::make_shared<mono::Window>(window_title, width, height, false, true);
    }
    else  // Release Build
    {
        // fullscreen, no-vsync
        m_window = std::make_shared<mono::Window>(window_title, width, height, true, false);
    }
    ResourceManager::window = m_window;

    m_timer = std::make_shared<Timer>();
    ResourceManager::timer = m_timer;

    m_limiter = std::make_shared<FramerateLimiter>();
    m_limiter->setLimit(300);
    ResourceManager::framerateLimiter = m_limiter;

    this->initFonts();
    this->initTextures();

    MainMenuStyle();

    m_sectionManager.emplaceSection<MainMenuSection>();
}

App::~App() noexcept
{
    this->destroyFonts();
    this->destroyTextures();
    ResourceManager::window.reset();
}

void App::initLogger() noexcept
{
    namespace fs = std::filesystem;
    fs::create_directory("../logs");

    constexpr mono::cstring info_pattern{"[%Y-%m-%d %T.%e][%^%l%$] %v"};
    constexpr mono::cstring debug_pattern{"[%Y-%m-%d %T.%e][%^%l%$][thread %t][%s:%#] %v"};
    spdlog::level log_level{spdlog::level::info};
    std::string log_pattern{info_pattern};

    if constexpr(mono::config::constant::debugMode)
    {
        log_level = spdlog::level::debug;
        log_pattern = debug_pattern;
    }

    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_pattern(log_pattern);
    console_sink->set_level(log_level);

    std::string file_name = std::format("../logs/{}", std::chrono::system_clock::now());
    file_name.replace(file_name.find(' '), 1, "_");
    file_name.replace(file_name.find(':'), 1, "-");
    file_name.replace(file_name.find(':'), 1, "-");
    file_name = file_name.substr(0, file_name.rfind('.'));
    if constexpr(mono::config::constant::debugMode)
    {
        file_name += "_debug";
    }
    file_name += ".log";

    {
        // create a file
        std::fstream file{file_name};
    }

    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(file_name, true);
    file_sink->set_pattern(log_pattern);
    file_sink->set_level(log_level);

    spdlog::logger multisink_logger("logger", {console_sink, file_sink});
    multisink_logger.set_level(log_level);

    spdlog::set_default_logger(std::make_shared<spdlog::logger>(multisink_logger));
    spdlog::debug("Logging initialized");
}

void App::initTextures() noexcept
{
    using res = ResourceManager;
    res::largeTreeTexture =
        Resource::create<mono::Texture>("../res/textures/large_tree.png", 128, 128);
    res::smallTreeTexture =
        Resource::create<mono::Texture>("../res/textures/small_tree.png", 64, 64);
    res::outdoorBenchTexture =
        Resource::create<mono::Texture>("../res/textures/outdoors_bench.png", 48, 16);
    res::chairTexture = Resource::create<mono::Texture>("../res/textures/chair.png", 16, 16);
    res::tableTexture = Resource::create<mono::Texture>("../res/textures/table.png", 32, 32);
    res::smallBushTexture =
        Resource::create<mono::Texture>("../res/textures/small_bush.png", 48, 48);
    res::largeBushTexture =
        Resource::create<mono::Texture>("../res/textures/large_bush.png", 56, 56);
    res::carTexture = Resource::create<mono::Texture>("../res/textures/car.png", 32, 64);
    res::destroyedCarTexture =
        Resource::create<mono::Texture>("../res/textures/destroyed_car.png", 32, 64);

    res::rifleInventoryTexture =
        Resource::create<mono::Texture>("../res/textures/gun_inventory.png", 256, 128);
    res::pistolInventoryTexture =
        Resource::create<mono::Texture>("../res/textures/pistol_inventory.png", 256, 128);

    res::enemyTexture = Resource::create<mono::Texture>(
        "../res/textures/entities/player.png",
        mono::TextureData{
            .widthTotal = 192,
            .heightTotal = 16,
            .widthSub = 16,
            .heightSub = 16,
            .numberOfSubs = 12,
            .numberOfSubsInOneRow = 12,
        });
}

void App::initFonts() noexcept
{
    using res = ResourceManager;

    const auto& window_width = res::window->getSize().x;

    // TODO(vis4rd): change title font size to resize dynamically or set it up just for fullscreen
    res::uiTitleFontSize =
        std::make_shared<float>(100.f * static_cast<float>(window_width) / 1920.f);
    res::uiTitleFont =
        std::make_shared<Font>("../res/fonts/prisma/Prisma.ttf", *res::uiTitleFontSize);
    // res::uiButtonFontSize = std::make_shared<float>(20.f * window_width / 1920.f);
    // res::uiButtonFont = std::make_shared<Font>("../res/fonts/abandoned/Abandoned-Bold.ttf",
    // *res::uiButtonFontSize);
    res::uiButtonFontSize =
        std::make_shared<float>(20.f * static_cast<float>(window_width) / 1920.f);
    res::uiButtonFont = std::make_shared<Font>(
        "../res/fonts/brass-mono/regular_comfortable.otf",
        *res::uiButtonFontSize);
    res::uiAmmoFontSize = std::make_shared<float>(25.f * static_cast<float>(window_width) / 1920.f);
    res::uiAmmoFont =
        std::make_shared<Font>("../res/fonts/gunplay/GUNPLAY_.ttf", *res::uiAmmoFontSize);
}

mono::Window& App::getWindow()
{
    return *m_window;
}

const mono::Window& App::getWindow() const
{
    return *m_window;
}

void App::run() noexcept
{
    spdlog::info("Starting main application loop");
    while(m_window->update(m_sectionManager))
    {
        m_timer->update();
        ResourceManager::framerateLimiter->wait();
        m_window->render(m_sectionManager);
    }
    spdlog::info("Halted main application loop");
}

void App::terminate(int code) noexcept
{
    spdlog::info("Closing the application");
}

void App::destroyTextures() noexcept
{
    using res = ResourceManager;
    res::carTexture.reset();
    res::destroyedCarTexture.reset();
    res::chairTexture.reset();
    res::outdoorBenchTexture.reset();
    res::tableTexture.reset();
    res::smallTreeTexture.reset();
    res::largeTreeTexture.reset();
    res::smallBushTexture.reset();
    res::largeBushTexture.reset();

    res::rifleInventoryTexture.reset();
    res::pistolInventoryTexture.reset();

    res::enemyTexture.reset();
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
