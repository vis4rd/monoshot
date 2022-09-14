#include "../../include/section/CreatorSection.hpp"

#include "../../include/renderer/Renderer.hpp"
#include "../../include/utility/ResourceManager.hpp"

#include <filesystem>

namespace fs = std::filesystem;

// globals
static std::size_t s_selected_texture_index = 0;
static bool s_selected_solid = false;
static glm::vec2 s_mouse_world_pos = {0.f, 0.f};

CreatorSection::CreatorSection()
    : Section(),
      m_map(5, 5),
      m_camera(glm::vec3(0.f, 0.f, 50.f), ResourceManager::window->getSize())
{
    m_name = "CreatorSection";

    // auto& input_manager = InputManager::get();
    // auto group_id = input_manager.addGroup(m_name);

    // input_manager.addKeybind(group_id,
    //     GLFW_KEY_ESCAPE,
    //     KeyState::PRESS_ONCE,
    //     []
    //     {
    //         SectionManager::get().popSection();
    //     });

    // input_manager.addKeybind(group_id,
    //     GLFW_KEY_A,
    //     KeyState::HOLD,
    //     [&camera = m_camera]
    //     {
    //         const auto& pos = camera.getPosition();
    //         camera.setPosition({pos.x - 0.01f * pos.z, pos.y, pos.z});
    //         camera.setTarget({pos.x, pos.y, 0.f});
    //     });

    // input_manager.addKeybind(group_id,
    //     GLFW_KEY_D,
    //     KeyState::HOLD,
    //     [&camera = m_camera]
    //     {
    //         const auto& pos = camera.getPosition();
    //         camera.setPosition({pos.x + 0.01f * pos.z, pos.y, pos.z});
    //         camera.setTarget({pos.x, pos.y, 0.f});
    //     });

    // input_manager.addKeybind(group_id,
    //     GLFW_KEY_W,
    //     KeyState::HOLD,
    //     [&camera = m_camera]
    //     {
    //         const auto& pos = camera.getPosition();
    //         camera.setPosition({pos.x, pos.y + 0.01f * pos.z, pos.z});
    //         camera.setTarget({pos.x, pos.y, 0.f});
    //     });

    // input_manager.addKeybind(group_id,
    //     GLFW_KEY_S,
    //     KeyState::HOLD,
    //     [&camera = m_camera]
    //     {
    //         const auto& pos = camera.getPosition();
    //         camera.setPosition({pos.x, pos.y - 0.01f * pos.z, pos.z});
    //         camera.setTarget({pos.x, pos.y, 0.f});
    //     });

    // // toggle solid factor of a tile
    // input_manager.addKeybind(group_id,
    //     GLFW_KEY_B,
    //     KeyState::PRESS_ONCE,
    //     []
    //     {
    //         s_selected_solid = !s_selected_solid;
    //     });

    // // place a tile
    // input_manager.addKeybind(group_id,
    //     GLFW_MOUSE_BUTTON_LEFT,
    //     KeyState::PRESS_ONCE,
    //     [&map = m_map, &pos = s_mouse_world_pos]
    //     {
    //         spdlog::debug("Placing a tile: {}, on {} with texture slot '{}'", s_selected_solid ? "solid" : "non-solid", util::vec2str(s_mouse_world_pos), s_selected_texture_index);
    //         map.setTile(pos.x, pos.y, 0.f, s_selected_texture_index, s_selected_solid);
    //     });

    // zooming of the view on mouse scroll
    auto window = ResourceManager::window->getNativeWindow();
    glfwSetWindowUserPointer(window, static_cast<void*>(&m_camera));
    glfwSetScrollCallback(window,
        [](GLFWwindow* window, double xoffset, double yoffset)
        {
            auto camera = static_cast<PerspectiveCamera*>(glfwGetWindowUserPointer(window));
            const auto& pos = camera->getPosition();
            const auto& delta_time = ResourceManager::timer->deltaTime();

            auto new_pos_z = pos.z - yoffset * delta_time * 100.f /* zoom velocity*/;
            if(new_pos_z < 0.1f)
            {
                new_pos_z = 0.1f;
            }
            else if(new_pos_z > 100.f)
            {
                new_pos_z = 100.f;
            }
            camera->setPosition({pos.x, pos.y, new_pos_z});
        });

    Renderer::init();

    auto texture_dir = fs::path("../res/textures/");
    for(const auto& file : fs::directory_iterator(texture_dir))
    {
        spdlog::trace("file: '{}'", file.path().string());
        spdlog::trace("extension: '{}', condition: {}", fs::path(file).extension().string(), (fs::path(file).extension() == ".png"));
        if(fs::path(file).extension() == ".png")
        {
            spdlog::debug("Found texture: '{}'", file.path().string());
            m_map.emplaceTexture(16, 16, file.path().string());
        }
    }
}

CreatorSection::~CreatorSection()
{
    auto window = ResourceManager::window->getNativeWindow();
    glfwSetWindowUserPointer(window, nullptr);
    glfwSetScrollCallback(window, nullptr);
    Renderer::shutdown();
    InputManager::get().removeGroup(m_name);
}

void CreatorSection::update() noexcept
{
    auto& input = InputManager::get();
    auto* window = ResourceManager::window->getNativeWindow();
    auto& pos = m_camera.getPosition();
    auto& delta_time = ResourceManager::timer->deltaTime();
    const double base_velocity = 1.f;
    const double velocity = base_velocity * delta_time * pos.z;
    if(input.isPressedOnce(GLFW_KEY_ESCAPE))
    {
        spdlog::debug("Lets pop some sections");
        SectionManager::get().popSection();
    }
    if(input.isHeld(GLFW_KEY_A))
    {
        m_camera.setPosition({pos.x - velocity, pos.y, pos.z});
        m_camera.setTarget({pos.x, pos.y, 0.f});
    }
    if(input.isHeld(GLFW_KEY_D))
    {
        m_camera.setPosition({pos.x + velocity, pos.y, pos.z});
        m_camera.setTarget({pos.x, pos.y, 0.f});
    }
    if(input.isHeld(GLFW_KEY_W))
    {
        m_camera.setPosition({pos.x, pos.y + velocity, pos.z});
        m_camera.setTarget({pos.x, pos.y, 0.f});
    }
    if(input.isHeld(GLFW_KEY_S))
    {
        m_camera.setPosition({pos.x, pos.y - velocity, pos.z});
        m_camera.setTarget({pos.x, pos.y, 0.f});
    }
    if(input.isPressedOnce(GLFW_KEY_B))
    {
        s_selected_solid = !s_selected_solid;
    }
    if(!ImGui::GetIO().WantCaptureMouse)
    {
        if(input.isPressedOnce(GLFW_MOUSE_BUTTON_LEFT))
        {
            // spdlog::debug("Placing a tile: {}, on {} with texture slot '{}'", s_selected_solid ? "solid" : "non-solid", util::vec2str(s_mouse_world_pos), s_selected_texture_index);
            m_map.setTile(s_mouse_world_pos.x, s_mouse_world_pos.y, 0.f, s_selected_texture_index, s_selected_solid);
        }
    }
}

void CreatorSection::render() noexcept
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // map rendering
    m_map.render(true, true);
    ShaderManager::getShader("quad").uploadMat4("uProjection", m_camera.getProjectionMatrix(), 0);
    ShaderManager::getShader("quad").uploadMat4("uView", m_camera.getViewMatrix(), 1);

    // hovered tile highlight
    Renderer::beginBatch();
    Renderer::drawQuad({std::round(s_mouse_world_pos.x), std::round(s_mouse_world_pos.y)}, {1.f, 1.f}, 0.f, {0.9f, 0.9f, 1.f, 0.2f});
    // Renderer::drawRect({std::round(s_mouse_world_pos.x), std::round(s_mouse_world_pos.y)}, {1.f, 1.f}, 0.f, {1.f, 1.f, 1.f, 1.f});
    // Renderer::drawRect({-5.f, 5.f}, {5.f, 5.f}, {5.f, -5.f}, {-5.f, -5.f}, {0.f, 1.f, 0.f, 1.f});
    // Renderer::drawRect({-3.f, 3.f}, {3.f, -3.f}, {0.f, 1.f, 0.f, 1.f});
    // Renderer::drawLine({0.f, 0.f}, {std::round(s_mouse_world_pos.x), std::round(s_mouse_world_pos.y)}, {1.f, 0.f, 0.f, 1.f});
    // Renderer::drawLine({7.f, 7.f}, {7.f, -7.f}, {0.f, 0.f, 1.f, 1.f}, {1.f, 1.f, 1.f, 1.f});
    Renderer::endBatch();
    ShaderManager::getShader("quad").uploadMat4("uProjection", m_camera.getProjectionMatrix(), 0);
    ShaderManager::getShader("quad").uploadMat4("uView", m_camera.getViewMatrix(), 1);

    glDisable(GL_BLEND);

    const auto& camera_pos = m_camera.getPosition();
    const auto& camera_target = m_camera.getTargetPosition();
    const glm::vec2 mouse_screen_pos = ResourceManager::window->getMousePosition();
    s_mouse_world_pos = this->mouseScreenPosToWorldPos(mouse_screen_pos, m_camera);
    ImGui::Begin("Camera options");
    {
        float zoom = camera_pos.z;
        if(ImGui::SliderFloat("camera zoom", &zoom, 0.1f, 200.f, "x%.1f"))
        {
            m_camera.setPosition({camera_pos.x, camera_pos.y, zoom});
        }
        if(ImGui::Button("Set position to map center"))
        {
            m_camera.setPosition({0.f, 0.f, camera_pos.z});
            m_camera.setTarget({0.f, 0.f, 0.f});
        }

        ImGui::Separator();
        ImGui::Text("Camera position: (%.2f, %.2f, %.2f)", camera_pos.x, camera_pos.y, camera_pos.z);
        ImGui::Text("Camera target: (%.2f, %.2f, %.2f)", camera_target.x, camera_target.y, camera_target.z);
    }
    ImGui::End();

    ImGui::Begin("Section options");
    {
        const std::size_t i = std::llroundf(s_mouse_world_pos.x);
        const std::size_t j = std::llroundf(s_mouse_world_pos.y);
        ImGui::Text("Hovered tile: (%lld, %lld)", i, j);
        ImGui::Text("Mouse screen position: (%.2f, %.2f)", mouse_screen_pos.x, mouse_screen_pos.y);
        ImGui::Text("Mouse world position: (%.2f, %.2f)", s_mouse_world_pos.x, s_mouse_world_pos.y);
        ImGui::Text("Time since app start: %lf", ResourceManager::timer->getTotalTime());
        ImGui::Text("Time since last frame: %lf", ResourceManager::timer->deltaTime());

        ImGui::Separator();
        static std::string preview = "Choose a texture";
        bool check = false;
        const auto& textures = m_map.getTextures();
        if(ImGui::BeginCombo("Textures", preview.c_str()))
        {
            for(std::size_t i = 0; i < textures.size(); i++)
            {
                if(ImGui::Selectable((textures[i]->getSourcePath() + "##unique_id").c_str(), &check))
                {
                    spdlog::debug("Chosen texture slot '{}' with path '{}'", i, textures[i]->getSourcePath());
                    s_selected_texture_index = i;
                    preview = textures[i]->getSourcePath() + "##unique_id";
                }
            }
            ImGui::EndCombo();
        }

        ImGui::Checkbox("Solid tile", &s_selected_solid);
        if(ImGui::Button("Save to file"))
        {
            m_map.saveToFile("testMap.map");
        }
    }
    ImGui::End();
}

// returns world coordinates at 0 height
glm::vec2 CreatorSection::mouseScreenPosToWorldPos(const glm::vec2& mouse_pos, Camera& camera)
{
    const auto& inverse_projection_matrix = camera.getInverseProjectionMatrix();
    const auto& inverse_view_matrix = camera.getInverseViewMatrix();
    spdlog::trace("Inverse projection matrix = {}", util::mat4str(inverse_projection_matrix));
    spdlog::trace("Inverse view matrix = {}", util::mat4str(inverse_view_matrix));

    const auto& mouse_x = mouse_pos.x;
    const auto& mouse_y = mouse_pos.y;
    const auto& window_size = ResourceManager::window->getSize();
    const auto& window_w = window_size.x;
    const auto& window_h = window_size.y;
    spdlog::trace("Window size = ({}, {})", window_w, window_h);

    const float norm_mouse_x = (2.f * mouse_x / window_w) - 1.f;
    const float norm_mouse_y = 1.f - (2.f * mouse_y / window_h);
    spdlog::trace("Normalized mouse position = ({}, {})", norm_mouse_x, norm_mouse_y);

    glm::vec3 norm_mouse_vector = glm::vec3(norm_mouse_x, norm_mouse_y, 1.f);
    glm::vec4 ray_clip = glm::vec4(norm_mouse_vector.x, norm_mouse_vector.y, -1.f, 1.f);
    glm::vec4 ray_eye = inverse_projection_matrix * ray_clip;
    ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.f, 0.f);
    glm::vec3 ray_world = glm::vec3((inverse_view_matrix * ray_eye));
    ray_world = glm::normalize(ray_world);
    spdlog::trace("Ray world = {}", util::vec3str(ray_world));

    float l = -(camera.getPosition().z / ray_world.z);
    spdlog::trace("L = {}", l);

    return {camera.getPosition().x + l * ray_world.x, camera.getPosition().y + l * ray_world.y};
}
