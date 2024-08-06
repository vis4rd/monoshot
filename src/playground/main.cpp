#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <imgui/imgui.h>
#include <input/InputManager.hpp>
#include <log/Logging.hpp>
#include <opengl/target/RenderWindow.hpp>
#include <renderer/Renderer.hpp>
#include <renderer/pass/InstancedQuadRenderPass.hpp>

int main()
{
    util::enableOpenGlLogging();
    spdlog::default_logger()->set_level(spdlog::level::debug);

    auto window = std::make_shared<mono::gl::RenderWindow>(1280, 720, "PLAYGROUND");
    window->setBorderlessFullscreen();

    mono::renderer::initialize(window);

    auto& quad_pass =
        mono::renderer::getDefaultPipeline().getRenderPass<mono::renderer::InstancedQuadRenderPass>(
            "quad");

    auto& input_manager = InputManager::get();
    std::vector<glm::vec2> points;
    std::vector<std::size_t> quad_ids;

    while(true)
    {
        glfwPollEvents();
        if(window->shouldClose())
        {
            break;
        }
        if(input_manager.isHeld(GLFW_MOUSE_BUTTON_LEFT)
           or input_manager.isPressedOnce(GLFW_MOUSE_BUTTON_RIGHT))
        {
            spdlog::debug("CLICKED LMB OR RMB");
            const auto pos = window->getMousePosition();
            points.push_back(pos);
            auto id = quad_pass.addQuad(pos, {10, 10}, 0, {1.f, 0.f, 0.f, 1.f});
            quad_ids.push_back(id);
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
        if(input_manager.isPressedOnce(GLFW_KEY_ESCAPE))
        {
            spdlog::debug("CLICKED ESCAPE");
            window->requestClose();
        }
        if(input_manager.isPressedOnce(GLFW_KEY_V))
        {
            spdlog::debug("CLICKED V");
            window->setVerticalSync(not window->isVerticalSyncEnabled());
        }
        if(input_manager.isPressedOnce(GLFW_KEY_1))
        {
            spdlog::debug("CLICKED 1");
            if(not quad_ids.empty())
            {
                const auto id = quad_ids.front();
                quad_ids.erase(quad_ids.begin());
                points.erase(points.begin());

                quad_pass.removeQuad(id);
            }
        }
        if(input_manager.isPressedOnce(GLFW_KEY_2))
        {
            spdlog::debug("CLICKED 2");
            if(quad_ids.size() >= 2)
            {
                for(int i = 0; i < 2; i++)
                {
                    const auto id = quad_ids.back();
                    quad_ids.pop_back();
                    points.pop_back();
                    quad_pass.removeQuad(id);
                }
            }
        }
        if(input_manager.isPressedOnce(GLFW_KEY_3))
        {
            spdlog::debug("CLICKED 3");
            if(quad_ids.size() >= 50)
            {
                for(int i = 0; i < 50; i++)
                {
                    const auto id = quad_ids.back();
                    quad_ids.pop_back();
                    points.pop_back();
                    quad_pass.removeQuad(id);
                }
            }
        }

        window->prepareRender();

        if(ImGui::Begin("Playground"))
        {
            ImGui::Text("fullscreen: %s", window->isFullscreen() ? "true" : "false");
            ImGui::Text("borderless: %s", window->isBorderlessFullscreen() ? "true" : "false");
            ImGui::Text("v-sync: %s", window->isVerticalSyncEnabled() ? "true" : "false");
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
            ImGui::Text("quad count: %d", points.size());
            ImGui::Text("frame time: %.3fms", 1000.0f / ImGui::GetIO().Framerate);
            ImGui::Text("fps: %.0f", ImGui::GetIO().Framerate);
            ImGui::End();
        }

        window->render();
    }

    mono::renderer::terminate();

    return 0;
}
