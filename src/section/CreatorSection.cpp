#include "../../include/section/CreatorSection.hpp"

#include "../../include/renderer/Renderer.hpp"
#include "../../include/utility/RandomNumber.hpp"
#include "../../include/utility/ResourceManager.hpp"

// globals
static std::size_t s_selected_map_item = BlockID::Wall;
static bool s_selected_solid = false;
static glm::vec2 s_mouse_world_pos = {0.f, 0.f};
static float s_randomized_rotation = random::getRandomNumber(0.f, 360.f);

CreatorSection::CreatorSection()
    : Section(),
      m_map(5, 5),
      m_camera(glm::vec3(0.f, 0.f, 50.f), ResourceManager::window->getSize())
{
    m_name = "CreatorSection";

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

    // renderer
    Renderer::init();
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
    if(input.isPressedOnce(GLFW_KEY_R))
    {
        s_randomized_rotation = std::fmod((std::floor(s_randomized_rotation / 45.f) + 1.f) * 45.f, 360.f);  // nudge rotation values to every 45 degrees
    }
    if(input.isHeld(GLFW_KEY_DELETE))
    {
        m_map.removeTile(s_mouse_world_pos.x, s_mouse_world_pos.y);
        m_map.removeObject(s_mouse_world_pos);
    }
    if(!ImGui::GetIO().WantCaptureMouse)
    {
        if(s_selected_map_item > ObjectID::FIRST_OBJECT && s_selected_map_item < ObjectID::LAST_OBJECT)  // if the chosen object is a MapObject
        {
            if(input.isPressedOnce(GLFW_MOUSE_BUTTON_LEFT))
            {
                m_map.addObject(s_mouse_world_pos, s_randomized_rotation, static_cast<ObjectID>(s_selected_map_item));
                s_randomized_rotation = random::getRandomNumber(0.f, 360.f);
            }
        }
        else if(s_selected_map_item > BlockID::FIRST_BLOCK && s_selected_map_item < BlockID::LAST_BLOCK)  // if its a Block
        {
            if(input.isHeld(GLFW_MOUSE_BUTTON_LEFT))
            {
                m_map.setTile(s_mouse_world_pos.x, s_mouse_world_pos.y, 0.f, static_cast<BlockID>(s_selected_map_item), s_selected_solid);
            }
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


    Renderer::beginBatch();
    if(s_selected_map_item > ObjectID::FIRST_OBJECT && s_selected_map_item < ObjectID::LAST_OBJECT)
    {
        // hovered object highlight
        const auto map_object = MapObject::createPredefined(static_cast<ObjectID>(s_selected_map_item), s_mouse_world_pos, s_randomized_rotation);
        if(map_object.getTexture())  // TODO: remove this branch when all textures are initialized
        {
            Renderer::drawQuad(map_object.getPosition(), map_object.getSize(), map_object.getRotation(), map_object.getTexture(), {0.9f, 0.9f, 1.f, 0.2f});
        }
    }
    else if(s_selected_map_item > BlockID::FIRST_BLOCK && s_selected_map_item < BlockID::LAST_BLOCK)
    {
        // hovered tile highlight
        Renderer::drawQuad({std::round(s_mouse_world_pos.x), std::round(s_mouse_world_pos.y)}, {1.f, 1.f}, 0.f, {0.9f, 0.9f, 1.f, 0.2f});
    }
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
        static std::string preview = "Wall";
        bool check = false;
        if(ImGui::BeginCombo("Blocks", preview.c_str()))
        {
            for(std::size_t block_id = BlockID::FIRST_BLOCK + 1; block_id < BlockID::LAST_BLOCK; block_id++)
            {
                if(ImGui::Selectable((blockToString(block_id) + std::string("##unique_id")).c_str(), &check))
                {
                    spdlog::debug("Selected Block '{}'", blockToString(block_id));
                    s_selected_map_item = block_id;
                    preview = blockToString(block_id);
                }
            }
            for(std::size_t object_id = ObjectID::FIRST_OBJECT + 1; object_id < ObjectID::LAST_OBJECT; object_id++)
            {
                if(ImGui::Selectable((objectIdToString(object_id) + std::string("##unique_id")).c_str(), &check))
                {
                    spdlog::debug("Selected MapObject '{}'", objectIdToString(object_id));
                    s_selected_map_item = object_id;
                    preview = objectIdToString(object_id);
                }
            }
            ImGui::EndCombo();
        }

        ImGui::Checkbox("Solid tile", &s_selected_solid);
        if(ImGui::Button("Save to file"))
        {
            m_map.saveToFile("testMap.map");
        }

        ImGui::SliderFloat("Object rotation", &s_randomized_rotation, 0.f, 360.f);
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
