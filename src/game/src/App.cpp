#include "../include/App.hpp"

#include <filesystem>

#include <config/Config.hpp>
#include <cstring/cstring.hpp>
#include <opengl/texture/Texture.hpp>
#include <renderer/pass/ImmediateLineRenderPass.hpp>
#include <renderer/pass/ImmediateQuadRenderPass.hpp>
#include <resource/Resource.hpp>
#include <resource/ResourceManager.hpp>
#include <ui/Font.hpp>

#include "../include/section/MainMenuSection.hpp"

App::App(const std::string& window_title, uint32_t width, uint32_t height)
    : m_sectionManager(SectionManager::get())
{
    spdlog::info("App version: {}", MONOSHOT_VERSION);

    m_window = std::make_shared<mono::gl::RenderWindow>(width, height, window_title);
    ResourceManager::window = m_window;

    {
        // custom pipeline
        auto& shader_manager = mono::gl::ShaderManager::get();
        auto& quad_shader = shader_manager.addShaderProgram(
            "quad",
            "../res/shaders/quad.vert",
            "../res/shaders/quad.frag");
        auto& line_shader = shader_manager.addShaderProgram(
            "line",
            "../res/shaders/line.vert",
            "../res/shaders/line.frag");

        auto pipeline = mono::renderer::RenderPipeline(0);
        pipeline.addRenderPass<mono::renderer::ImmediateQuadRenderPass>(
            "quad",
            m_window,
            quad_shader);
        pipeline.addRenderPass<mono::renderer::ImmediateLineRenderPass>(
            "line",
            m_window,
            line_shader);
        mono::renderer::addPipeline(std::move(pipeline));
    }

    // TODO(vis4rd): set window configuration from config.ini
    m_window->setFullscreen(true);
    m_window->setVerticalSync(false);

    m_timer = std::make_shared<Timer>();
    ResourceManager::timer = m_timer;

    this->initFonts();
    this->initTextures();

    MainMenuStyle();

    m_sectionManager.emplaceSection<MainMenuSection>();
}

App::~App() noexcept
{
    mono::renderer::terminate();
    this->destroyFonts();
    this->destroyTextures();
    ResourceManager::window.reset();
}

void App::initTextures() noexcept
{
    spdlog::info("Loading textures");
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
    spdlog::info("Loading fonts");
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

void App::run() noexcept
{
    spdlog::info("Starting main application loop");
    while(true)
    {
        this->update(m_sectionManager);
        if(m_window->shouldClose())
        {
            break;
        }
        m_timer->update();
        this->render(m_sectionManager);
    }
    spdlog::info("Stopped main application loop");
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
