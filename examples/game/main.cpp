#include <mono/config/Config.hpp>
#include <mono/dev_ui/DevUI.hpp>
#include <mono/input/Input.hpp>
#include <mono/log/Logging.hpp>
#include <mono/renderer/RenderPipeline.hpp>
#include <mono/renderer/RenderTexture.hpp>
#include <mono/renderer/RenderWindow.hpp>
#include <mono/renderer/Renderer.hpp>
#include <mono/renderer/pass/ImmediateLineRenderPass.hpp>
#include <mono/renderer/pass/ImmediateQuadRenderPass.hpp>
#include <mono/renderer/pass/PostProcessPass.hpp>
#include <opengl/shader/ShaderManager.hpp>

int main(int, char**)
{
    auto& log_level_config =
        mono::config::runtime.addConfigItem<mono::config::OptionStringConfigItem>(
            std::string{"engine"},
            std::string{"LogLevel"},
            std::vector<std::string>{"trace", "debug", "info", "warn", "error", "critical"});
    const auto log_level_cb_guard =
        log_level_config.setOnSetCallback([](std::string_view, std::string_view new_value) {
            spdlog::set_level(spdlog::level_from_str(std::string{new_value}));
        });
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

    mono::config::initialize("../examples/game/config.ini");
    mono::log::initialize();


    if(not mono::config::runtime.validate())
    {
        mono::log::error("Config validation failed");
        return EXIT_FAILURE;
    }

    const auto resolution =
        resolution_config.getValue<glm::ivec2>().value_or(glm::ivec2{1920, 1080});

    auto window = std::make_shared<mono::RenderWindow>(resolution.x, resolution.y, "Monoshot app");
    // BUG: window resizing does not resize the RenderTexture
    auto render_texture = std::make_shared<mono::RenderTexture>(resolution.x, resolution.y);

    mono::dev_ui::initialize();

    constexpr std::int32_t pipeline_id = 0;
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
        auto& all_white_shader = shader_manager.addShaderProgram(
            "all_white",
            "../res/shaders/post_process.vert",
            "../res/shaders/all_white.frag");

        auto pipeline = mono::renderer::RenderPipeline(pipeline_id);
        pipeline.addRenderPass<mono::renderer::ImmediateQuadRenderPass>(
            "quad",
            render_texture,
            quad_shader);
        pipeline.addRenderPass<mono::renderer::ImmediateLineRenderPass>(
            "line",
            render_texture,
            line_shader);
        pipeline.addRenderPass<mono::renderer::test::PostProcessPass>(
            "all_white",
            window,
            all_white_shader);
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


    auto& quad_pass = mono::renderer::getPipeline(pipeline_id)
                          .getRenderPass<mono::renderer::ImmediateQuadRenderPass>("quad");
    auto& line_pass = mono::renderer::getPipeline(pipeline_id)
                          .getRenderPass<mono::renderer::ImmediateLineRenderPass>("line");
    auto& all_white_pass = mono::renderer::getPipeline(pipeline_id)
                               .getRenderPass<mono::renderer::test::PostProcessPass>("all_white");

    const auto refresh_projection_view = [&window, &quad_pass, &line_pass]() {
        const auto resolution = window->getSize();
        const auto projection = glm::ortho(
            0.f,
            static_cast<float>(resolution.x),
            static_cast<float>(resolution.y),
            0.f,
            -2000.f,
            2000.f);

        const auto view = glm::lookAt(
            glm::vec3{0.f, 0.f, 1.f},
            glm::vec3{0.f, 0.f, 0.f},
            glm::vec3{0.f, 1.f, 0.f});
        quad_pass.setProjection(projection);
        quad_pass.setView(view);
        line_pass.setProjection(projection);
        line_pass.setView(view);
    };

    refresh_projection_view();

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

        quad_pass.drawQuad(window->getMousePosition(), {15.f, 15.f}, 0.f, {1.f, 0.f, 0.f, 1.f});
        line_pass.drawLine(
            {200.f, 200.f},
            window->getMousePosition(),
            {0.f, 1.f, 0.f, 1.f},
            {0.f, 1.f, 0.f, 1.f});

        all_white_pass.drawTexture(
            render_texture->getID(),
            render_texture->getSize().x,
            render_texture->getSize().y);

        refresh_projection_view();
        window->prepareRender();

        ImGui::Begin("Debug info");
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::Text(
            "Mouse Position: (%.1f, %.1f)",
            window->getMousePosition().x,
            window->getMousePosition().y);
        ImGui::Text("Window Size: (%d, %d)", window->getSize().x, window->getSize().y);
        ImGui::End();

        // workaround for accumulating draws in RenderTexture
        // it is happening, because two RenderPasses draw to the same RenderTexture without clearing
        // fix: merge the two RenderPasses into one or use two RenderTextures
        render_texture->activate();
        ::gl::glClear(::gl::GL_COLOR_BUFFER_BIT | ::gl::GL_STENCIL_BUFFER_BIT);
        render_texture->deactivate();
        // end of workaround

        mono::renderer::render();

        window->render();
    }

    mono::log::info("Closing the application");
    return EXIT_SUCCESS;
}
