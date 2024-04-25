#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <imgui/imgui.h>
#include <input/InputManager.hpp>
#include <log/Logging.hpp>
#include <opengl/target/RenderWindow.hpp>
#include <renderer/Renderer.hpp>

int main()
{
    util::enableOpenGlLogging();
    spdlog::default_logger()->set_level(spdlog::level::debug);

    mono::gl::RenderWindow window(1280, 720, "PLAYGROUND");
    window.setBorderlessFullscreen();
    auto& input_manager = InputManager::get();

    std::vector<glm::vec2> points;

    while(true)
    {
        glfwPollEvents();
        if(window.shouldClose())
        {
            break;
        }
        if(input_manager.isHeld(GLFW_MOUSE_BUTTON_LEFT))
        {
            const auto pos = window.getMousePosition();
            points.push_back(pos);
        }
        if(input_manager.isPressedOnce(GLFW_KEY_F11))
        {
            window.toggleBorderlessFullscreen();
        }
        if(input_manager.isPressedOnce(GLFW_KEY_F10))
        {
            window.toggleFullscreen();
        }
        if(input_manager.isPressedOnce(GLFW_KEY_ESCAPE))
        {
            window.requestClose();
        }
        if(input_manager.isPressedOnce(GLFW_KEY_V))
        {
            window.setVerticalSync(not window.isVerticalSyncEnabled());
        }

        window.prepareRender();

        for(const auto& point : points)
        {
            mono::renderer::drawQuad(point, {10, 10}, 0, {1.f, 0.f, 0.f, 1.f});
        }

        if(ImGui::Begin("Playground"))
        {
            ImGui::Text("fullscreen: %s", window.isFullscreen() ? "true" : "false");
            ImGui::Text("borderless: %s", window.isBorderlessFullscreen() ? "true" : "false");
            ImGui::Text("v-sync: %s", window.isVerticalSyncEnabled() ? "true" : "false");
            ImGui::End();
        }

        mono::renderer::render(
            glm::ortho(0.f, window.getSize().x + 0.f, window.getSize().y + 0.f, 0.f, 0.1f, 1000.f),
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

        window.render();
    }

    return 0;
}
