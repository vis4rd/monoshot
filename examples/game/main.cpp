#include <mono/config/Config.hpp>
#include <mono/dev_ui/DevUI.hpp>
#include <mono/input/Input.hpp>
#include <mono/log/Logging.hpp>
#include <mono/renderer/RenderPipeline.hpp>
#include <mono/renderer/Renderer.hpp>
#include <mono/renderer/pass/ImmediateLineRenderPass.hpp>
#include <mono/renderer/pass/ImmediateQuadRenderPass.hpp>
#include <opengl/shader/ShaderManager.hpp>
#include <opengl/target/RenderWindow.hpp>

int main(int, char**)
{
    mono::config::initialize();
    mono::log::initialize();

    auto& window_mode_config =
        mono::config::runtime.addConfigItem<mono::config::OptionStringConfigItem>(
            std::string{"app.window"},
            std::string{"Mode"},
            std::vector<std::string>{"windowed", "fullscreen", "borderless"});
    auto& vsync_config = mono::config::runtime.addConfigItem<mono::config::BasicConfigItem<bool>>(
        "app.window",
        "UseVSync");
    auto& resolution_config =
        mono::config::runtime
            .addConfigItem<mono::config::MultiNumberConfigItem<2, std::int32_t, 'x'>>(
                "app.window",
                "Resolution");

    if(not mono::config::runtime.validate())
    {
        mono::log::error("Config validation failed");
        return EXIT_FAILURE;
    }

    const auto resolution =
        resolution_config.getValue<glm::ivec2>().value_or(glm::ivec2{1920, 1080});

    auto window =
        std::make_shared<mono::gl::RenderWindow>(resolution.x, resolution.y, "Monoshot app");

    mono::dev_ui::initialize();

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
            window,
            quad_shader);
        pipeline.addRenderPass<mono::renderer::ImmediateLineRenderPass>(
            "line",
            window,
            line_shader);
        mono::renderer::addPipeline(std::move(pipeline));
    }

    const auto window_mode = window_mode_config.getValue<std::string>().value_or("borderless");

    if(window_mode.compare("borderless") == 0)
    {
        window->setBorderlessFullscreen();
    }
    else if(window_mode.compare("fullscreen") == 0)
    {
        window->setFullscreen();
    }
    else
    {
        window->setFullscreen(false);
    }

    const auto vsync_enabled = vsync_config.getValue<bool>().value_or(true);
    window->setVerticalSync(vsync_enabled);

    while(true)
    {
        mono::input::pollEvents();
        if(mono::input::isPressedOnce(GLFW_KEY_F11))
        {
            auto size = window->getSize();
            mono::log::info("on F11: window size = {}x{}", size.x, size.y);

            window->toggleFullscreen();
            size = window->getSize();
            mono::log::info("after F11: window size = {}x{}", size.x, size.y);
        }
        if(mono::input::isPressedOnce(GLFW_KEY_ESCAPE))
        {
            window->requestClose();
        }
        if(window->shouldClose())
        {
            break;
        }

        if(window->isMinimized() or window->shouldClose())
        {
            continue;
        }

        window->prepareRender();

        mono::renderer::render();

        window->render();
    }

    mono::log::info("Closing the application");
    return EXIT_SUCCESS;
}
