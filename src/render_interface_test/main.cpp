#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <imgui/imgui.h>
#include <input/InputManager.hpp>
#include <log/Logging.hpp>
#include <opengl/shader/ShaderManager.hpp>
#include <opengl/target/RenderWindow.hpp>
#include <renderer/RenderPipeline.hpp>
#include <renderer/Renderer.hpp>
#include <renderer/pass/ImmediateLineRenderPass.hpp>
#include <renderer/pass/ImmediateQuadRenderPass.hpp>
#include <renderer/pass/InstancedQuadRenderPass.hpp>

int main()
{
    util::enableOpenGlLogging();
    spdlog::default_logger()->set_level(spdlog::level::debug);

    auto window = std::make_shared<mono::gl::RenderWindow>(1280, 720, "RENDER_INTERFACE_TEST");
    window->setBorderlessFullscreen();

    mono::renderer::initialize(window);

    auto& input_manager = InputManager::get();
    auto& pipeline = mono::renderer::getDefaultPipeline();
    //
    // auto& quad_pass = pipeline.getRenderPass<mono::renderer::InstancedQuadRenderPass>("quad");
    // (void)quad_pass.addQuad({1000, 400}, {100, 100}, 0, {1.f, 0.f, 0.f, 1.f});
    //
    auto& quad_shader = mono::gl::ShaderManager::get().getShader("quad");
    pipeline.addRenderPass<mono::renderer::ImmediateQuadRenderPass>("quad2", window, quad_shader);
    auto& quad_pass = pipeline.getRenderPass<mono::renderer::ImmediateQuadRenderPass>("quad2");
    //

    auto& line_shader = mono::gl::ShaderManager::get().getShader("line");
    pipeline.addRenderPass<mono::renderer::ImmediateLineRenderPass>("line", window, line_shader);
    auto& line_pass = pipeline.getRenderPass<mono::renderer::ImmediateLineRenderPass>("line");

    while(true)
    {
        glfwPollEvents();
        if(window->shouldClose())
        {
            break;
        }
        if(input_manager.isPressedOnce(GLFW_KEY_ESCAPE))
        {
            spdlog::debug("CLICKED ESCAPE");
            window->requestClose();
        }
        if(input_manager.isPressedOnce(GLFW_KEY_F11))
        {
            spdlog::debug("CLICKED F11");
            window->toggleBorderlessFullscreen();
        }
        if(input_manager.isPressedOnce(GLFW_KEY_F10))
        {
            spdlog::debug("CLICKED F10");
            window->toggleFullscreen();
        }
        if(input_manager.isPressedOnce(GLFW_KEY_V))
        {
            spdlog::debug("CLICKED V");
            window->setVerticalSync(not window->isVerticalSyncEnabled());
        }

        //
        quad_pass.drawQuad({1000, 400}, {100, 100}, 0, {1.f, 0.f, 0.f, 1.f});
        //
        line_pass.drawLine({0, 0}, window->getSize(), {1.f, 0.f, 0.f, 1.f}, {0.f, 1.f, 0.f, 1.f});

        window->prepareRender();

        if(ImGui::Begin("Render Interface Test"))
        {
            ImGui::Text("fullscreen: %s", window->isFullscreen() ? "true" : "false");
            ImGui::Text("borderless: %s", window->isBorderlessFullscreen() ? "true" : "false");
            ImGui::Text("v-sync: %s", window->isVerticalSyncEnabled() ? "true" : "false");
            ImGui::Text(
                "Mouse Position: (%.0f, %.0f)",
                window->getMousePosition().x,
                window->getMousePosition().y);
            ImGui::End();
        }

        mono::renderer::render(
            glm::ortho(
                0.f,
                window->getSize().x + 0.f,
                window->getSize().y + 0.f,
                0.f,
                0.1f,
                1000.f),
            glm::lookAt(
                glm::vec3{0.f, 0.f, 100.f},
                glm::vec3{0.f, 0.f, 0.f},
                glm::vec3{0.f, 1.f, 0.f}));

        if(ImGui::Begin("Renderer Statistics"))
        {
            ImGui::NewLine();
            ImGui::Text("frame time: %.3fms", 1000.0f / ImGui::GetIO().Framerate);
            ImGui::Text("fps: %.0f", ImGui::GetIO().Framerate);
            ImGui::End();
        }

        window->render();
    }

    mono::renderer::terminate();

    return 0;
}
