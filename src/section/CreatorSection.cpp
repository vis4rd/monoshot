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
      m_camera(glm::vec3(0.f, 0.f, 50.f), {ResourceManager::window->getSize().first, ResourceManager::window->getSize().second})
{
    m_name = "CreatorSection";

    auto& input_manager = InputManager::get();
    auto group_id = input_manager.addGroup(m_name);

    input_manager.addKeybind(group_id,
        GLFW_KEY_ESCAPE,
        KeyState::PRESS_ONCE,
        []
        {
            SectionManager::get().popSection();
        });

    input_manager.addKeybind(group_id,
        GLFW_KEY_LEFT,
        KeyState::HOLD,
        [&camera = m_camera]
        {
            const auto& pos = camera.getPosition();
            camera.setPosition({pos.x - 0.01f * pos.z, pos.y, pos.z});
            camera.setTarget({pos.x, pos.y, 0.f});
        });

    input_manager.addKeybind(group_id,
        GLFW_KEY_RIGHT,
        KeyState::HOLD,
        [&camera = m_camera]
        {
            const auto& pos = camera.getPosition();
            camera.setPosition({pos.x + 0.01f * pos.z, pos.y, pos.z});
            camera.setTarget({pos.x, pos.y, 0.f});
        });

    input_manager.addKeybind(group_id,
        GLFW_KEY_UP,
        KeyState::HOLD,
        [&camera = m_camera]
        {
            const auto& pos = camera.getPosition();
            camera.setPosition({pos.x, pos.y + 0.01f * pos.z, pos.z});
            camera.setTarget({pos.x, pos.y, 0.f});
        });

    input_manager.addKeybind(group_id,
        GLFW_KEY_DOWN,
        KeyState::HOLD,
        [&camera = m_camera]
        {
            const auto& pos = camera.getPosition();
            camera.setPosition({pos.x, pos.y - 0.01f * pos.z, pos.z});
            camera.setTarget({pos.x, pos.y, 0.f});
        });

    // toggle solid factor of a tile
    input_manager.addKeybind(group_id,
        GLFW_KEY_B,
        KeyState::PRESS_ONCE,
        []
        {
            s_selected_solid = !s_selected_solid;
        });

    // place a tile
    input_manager.addKeybind(group_id,
        GLFW_MOUSE_BUTTON_LEFT,
        KeyState::PRESS_ONCE,
        [&map = m_map, &pos = s_mouse_world_pos]
        {
            spdlog::debug("Placing a tile: {}, on {} with texture slot '{}'", s_selected_solid ? "solid" : "non-solid", util::vec2str(s_mouse_world_pos), s_selected_texture_index);
            map.setTile(pos.x, pos.y, 0.f, s_selected_texture_index, s_selected_solid);
        });

    // zooming of the view on mouse scroll
    auto window = ResourceManager::window->getNativeWindow();
    glfwSetWindowUserPointer(window, static_cast<void*>(&m_camera));
    glfwSetScrollCallback(window,
        [](GLFWwindow* window, double xoffset, double yoffset)
        {
            auto camera = static_cast<PerspectiveCamera*>(glfwGetWindowUserPointer(window));
            const auto& pos = camera->getPosition();
            auto new_pos_z = pos.z - yoffset;
            if(new_pos_z < 0.1f)
            {
                new_pos_z = 0.1f;
            }
            camera->setPosition({pos.x, pos.y, new_pos_z});
        });

    // m_map.loadFromFile("../res/maps/new_debug_map.map");

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
}

void CreatorSection::update() noexcept { }

void CreatorSection::render() noexcept
{
    Renderer::beginBatch();
    Renderer::drawQuad({0.f, 0.f}, {m_map.getWidth(), m_map.getHeight()}, 0.f, {0.3f, 0.3f, 0.3f, 1.f});
    Renderer::drawQuad({0.f, 10.f}, {1.f, 1.f}, 0.f, {1.f, 0.5f, 0.5f, 1.f});
    Renderer::drawQuad({0.f, 8.f}, {1.f, 1.f}, 0.f, {1.f, 0.5f, 0.5f, 1.f});
    Renderer::drawQuad({9.f, 12.f}, {1.f, 1.f}, 45.f, {1.f, 0.5f, 0.5f, 1.f});
    Renderer::endBatch();
    ShaderManager::getShader("quad").uploadMat4("uProjection", m_camera.getProjectionMatrix(), 0);
    ShaderManager::getShader("quad").uploadMat4("uView", m_camera.getViewMatrix(), 1);
    
    m_map.render();

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
        const std::size_t center_x = m_map.getWidth() / 2;
        const std::size_t center_y = m_map.getHeight() / 2;

        const std::int32_t i = static_cast<std::int32_t>(std::round(s_mouse_world_pos.x) - 0.5f);
        const std::int32_t j = static_cast<std::int32_t>(std::round(s_mouse_world_pos.y) - 0.5f);
        ImGui::Text("Hovered tile: (%d, %d)", i, j);
        ImGui::Text("Mouse screen position: (%.2f, %.2f)", mouse_screen_pos.x, mouse_screen_pos.y);
        ImGui::Text("Mouse world position: (%.2f, %.2f)", s_mouse_world_pos.x, s_mouse_world_pos.y);

        // TODO(DONE): texture selection
        // TODO: change empty tile to display alpha = 0 color
        // TODO: texture placement
        // TODO: saving map to a file
        // TODO: move most of this stuff to update()
        ImGui::Separator();
        static std::string preview = "Choose a texture";
        bool check = false;
        const auto& textures = m_map.getTextures();
        if(ImGui::BeginCombo("Textures", preview.c_str()))
        {
            for(std::size_t i = 0; i < textures.size(); i++)
            {
                // ImGui::Selectable(texture.getSourcePath().c_str(), &check);
                if(ImGui::Selectable(textures[i]->getSourcePath().c_str(), &check))
                {
                    // m_map.setTile(std::round(mouse_world_pos.x) - 0.5f, std::round(mouse_world_pos.y) - 0.5f, 0.f);  // TODO: replace with actual texture slot
                    spdlog::debug("Chosen texture slot '{}' with path '{}'", i, textures[i]->getSourcePath());
                    s_selected_texture_index = i;
                    preview = textures[i]->getSourcePath();
                }
            }
            ImGui::EndCombo();
        }

        ImGui::Checkbox("Solid tile", &s_selected_solid);
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
    const auto& [window_w, window_h] = ResourceManager::window->getSize();
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
