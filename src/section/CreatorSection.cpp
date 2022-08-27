#include "../../include/section/CreatorSection.hpp"

#include "../../include/renderer/Renderer.hpp"
#include "../../include/utility/ResourceManager.hpp"

#include <filesystem>

namespace fs = std::filesystem;

static std::size_t s_selected_texture_index = 0;

CreatorSection::CreatorSection()
    : Section(),
      m_mapGrid(5, 5),
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
            camera.setPosition({pos.x - 0.1f, pos.y, pos.z});
            camera.setTarget({pos.x, pos.y, 0.f});
        });

    input_manager.addKeybind(group_id,
        GLFW_KEY_RIGHT,
        KeyState::HOLD,
        [&camera = m_camera]
        {
            const auto& pos = camera.getPosition();
            camera.setPosition({pos.x + 0.1f, pos.y, pos.z});
            camera.setTarget({pos.x, pos.y, 0.f});
        });

    input_manager.addKeybind(group_id,
        GLFW_KEY_UP,
        KeyState::HOLD,
        [&camera = m_camera]
        {
            const auto& pos = camera.getPosition();
            camera.setPosition({pos.x, pos.y + 0.1f, pos.z});
            camera.setTarget({pos.x, pos.y, 0.f});
        });

    input_manager.addKeybind(group_id,
        GLFW_KEY_DOWN,
        KeyState::HOLD,
        [&camera = m_camera]
        {
            const auto& pos = camera.getPosition();
            camera.setPosition({pos.x, pos.y - 0.1f, pos.z});
            camera.setTarget({pos.x, pos.y, 0.f});
        });

    m_mapGrid.loadFromFile("../res/maps/new_debug_map.map");

    // m_mapGrid.emplaceTexture(16, 16, "../res/textures/grass.png");

    Renderer::init();

    auto texture_dir = fs::path("../res/textures/");
    for(const auto& file : fs::directory_iterator(texture_dir))
    {
        spdlog::trace("file: '{}'", file.path().string());
        spdlog::trace("extension: '{}', condition: {}", fs::path(file).extension().string(), (fs::path(file).extension() == ".png"));
        if(fs::path(file).extension() == ".png")
        {
            spdlog::debug("Found texture: '{}'", file.path().string());
            m_mapGrid.emplaceTexture(16, 16, file.path().string());
        }
    }
}

CreatorSection::~CreatorSection()
{
    Renderer::shutdown();
}

void CreatorSection::update() noexcept { }

void CreatorSection::render() noexcept
{
    m_mapGrid.render();

    Renderer::beginBatch();
    Renderer::drawQuad({0.f, 10.f}, {1.f, 1.f}, 0.f, {1.f, 0.5f, 0.5f, 1.f});
    Renderer::drawQuad({0.f, 8.f}, {1.f, 1.f}, 0.f, {1.f, 0.5f, 0.5f, 1.f});
    Renderer::drawQuad({9.f, 12.f}, {1.f, 1.f}, 45.f, {1.f, 0.5f, 0.5f, 1.f});
    Renderer::endBatch();
    ShaderManager::getShader("quad").uploadMat4("uProjection", m_camera.getProjectionMatrix(), 0);
    ShaderManager::getShader("quad").uploadMat4("uView", m_camera.getViewMatrix(), 1);

    const auto& camera_pos = m_camera.getPosition();
    const auto& camera_target = m_camera.getTargetPosition();
    const glm::vec2 mouse_screen_pos = ResourceManager::window->getMousePosition();
    const auto mouse_world_pos = this->mouseScreenPosToWorldPos(mouse_screen_pos, m_camera);
    ImGui::Begin("Camera options");
    {
        static float zoom = camera_pos.z;
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
        const std::size_t center_x = m_mapGrid.getWidth() / 2;
        const std::size_t center_y = m_mapGrid.getHeight() / 2;

        const std::int32_t i = static_cast<std::int32_t>(std::round(mouse_world_pos.x) - 0.5f);
        const std::int32_t j = static_cast<std::int32_t>(std::round(mouse_world_pos.y) - 0.5f);
        ImGui::Text("Hovered tile: (%d, %d)", i, j);
        ImGui::Text("Mouse screen position: (%.2f, %.2f)", mouse_screen_pos.x, mouse_screen_pos.y);
        ImGui::Text("Mouse world position: (%.2f, %.2f)", mouse_world_pos.x, mouse_world_pos.y);

        // TODO: texture selection, texture placement, saving map to a file, move most of this stuff to update()
        ImGui::Separator();
        std::string preview = "Choose a texture";
        bool check = false;
        const auto& textures = m_mapGrid.getTextures();
        if(ImGui::BeginCombo("Textures", preview.data()))
        {
            for(std::size_t i = 0; i < textures.size(); i++)
            {
                // ImGui::Selectable(texture.getSourcePath().c_str(), &check);
                if(ImGui::Selectable(textures[i].getSourcePath().c_str(), &check))
                {
                    // m_mapGrid.setTile(std::round(mouse_world_pos.x) - 0.5f, std::round(mouse_world_pos.y) - 0.5f, 0.f);  // TODO: replace with actual texture slot
                    spdlog::debug("Chosen texture slot '{}' with path '{}'", i, textures[i].getSourcePath());
                    s_selected_texture_index = i;
                    preview = textures[i].getSourcePath();
                }
            }
            ImGui::EndCombo();
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
