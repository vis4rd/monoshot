#include "../../include/section/CreatorSection.hpp"

#include <renderer/Renderer.hpp>
#include <resource/ResourceManager.hpp>
#include <tinyfiledialogs/tinyfiledialogs.h>

#include "../../include/ecs/actions.hpp"
#include "../../include/ecs/components.hpp"
#include "../../include/utility/RandomNumber.hpp"

// globals
static std::size_t selectedMapItem = BlockID::Wall;
static bool selectedSolid = false;
static glm::vec2 mouseWorldPos = {0.f, 0.f};
static float randomizedRotation = util::random::getRandomNumber(0.f, 360.f);
static glm::vec2 endAreaSize = {10.f, 10.f};

CreatorSection::CreatorSection()
    : Section()
    , m_renderer()
    , m_map(m_renderer, 5, 5)
    , m_camera(glm::vec3(0.f, 0.f, 50.f), ResourceManager::window->getSize())
{
    m_name = "CreatorSection";

    // zooming of the view on mouse scroll
    auto window = ResourceManager::window->getNativeWindow();
    glfwSetWindowUserPointer(window, static_cast<void*>(&m_camera));
    glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {
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
}

CreatorSection::~CreatorSection()
{
    auto window = ResourceManager::window->getNativeWindow();
    glfwSetWindowUserPointer(window, nullptr);
    glfwSetScrollCallback(window, nullptr);
    InputManager::get().removeGroup(m_name);
}

void CreatorSection::update() noexcept
{
    auto& input = InputManager::get();
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
        selectedSolid = !selectedSolid;
    }
    if(input.isPressedOnce(GLFW_KEY_R))
    {
        randomizedRotation = std::fmod(
            (std::floor(randomizedRotation / 45.f) + 1.f) * 45.f,
            360.f);  // nudge rotation values to every 45 degrees
    }
    if(input.isHeld(GLFW_KEY_DELETE))
    {
        m_map.removeTile(mouseWorldPos.x, mouseWorldPos.y);
        m_map.removeObject(mouseWorldPos);
        for(auto&& [enemy, pos] : m_entities.view<const ecs::component::Position>().each())
        {
            const bool col = AABB::isColliding(mouseWorldPos, {0.01f, 0.01f}, pos, {1.f, 1.f});
            if(col)
            {
                m_entities.destroy(enemy);
            }
        }
    }
    if(!ImGui::GetIO().WantCaptureMouse)
    {
        if(selectedMapItem > ObjectID::FIRST_OBJECT
           && selectedMapItem < ObjectID::LAST_OBJECT)  // if the chosen object is a MapObject
        {
            if(input.isPressedOnce(GLFW_MOUSE_BUTTON_LEFT))
            {
                m_map.addObject(
                    mouseWorldPos,
                    randomizedRotation,
                    static_cast<ObjectID>(selectedMapItem));
                randomizedRotation = util::random::getRandomNumber(0.f, 360.f);
            }
        }
        else if(
            selectedMapItem > BlockID::FIRST_BLOCK
            && selectedMapItem < BlockID::LAST_BLOCK)  // if its a Block
        {
            if(input.isHeld(GLFW_MOUSE_BUTTON_LEFT))
            {
                m_map.setTile(
                    mouseWorldPos.x,
                    mouseWorldPos.y,
                    0.f,
                    static_cast<BlockID>(selectedMapItem),
                    selectedSolid);
            }
        }
        else if(selectedMapItem == 9999)
        {
            if(input.isPressedOnce(GLFW_MOUSE_BUTTON_LEFT))
            {
                m_map.setEndArea(mouseWorldPos, endAreaSize);
            }
        }
        else if(selectedMapItem == 10000)
        {
            if(input.isPressedOnce(GLFW_MOUSE_BUTTON_LEFT))
            {
                ecs::action::spawnEnemy(m_entities, mouseWorldPos, {1.f, 1.f}, randomizedRotation);
                spdlog::debug(
                    "Map: Placing an enemy with coords ({}, {})",
                    mouseWorldPos.x,
                    mouseWorldPos.y);
            }
        }
    }
    if(constexpr float sizeVel = 5.f; selectedMapItem == 9999)
    {
        if(input.isHeld(GLFW_KEY_UP))
        {
            endAreaSize.y += (static_cast<float>(delta_time) * sizeVel);
        }
        if(input.isHeld(GLFW_KEY_DOWN))
        {
            endAreaSize.y -= (static_cast<float>(delta_time) * sizeVel);
        }
        if(input.isHeld(GLFW_KEY_RIGHT))
        {
            endAreaSize.x += (static_cast<float>(delta_time) * sizeVel);
        }
        if(input.isHeld(GLFW_KEY_LEFT))
        {
            endAreaSize.x -= (static_cast<float>(delta_time) * sizeVel);
        }
    }
    endAreaSize = glm::max(endAreaSize, 1.f);
}

void CreatorSection::render() noexcept
{
    static bool draw_area = true;
    static bool draw_bbs = false;
    static bool draw_end_area = true;

    // map rendering
    m_map.render(
        m_camera.getProjectionMatrix(),
        m_camera.getViewMatrix(),
        draw_area,
        draw_bbs,
        draw_end_area);

    m_renderer.beginBatch();
    if(selectedMapItem > ObjectID::FIRST_OBJECT && selectedMapItem < ObjectID::LAST_OBJECT)
    {
        // hovered object highlight
        const auto map_object = MapObject::createPredefined(
            static_cast<ObjectID>(selectedMapItem),
            mouseWorldPos,
            randomizedRotation);
        if(map_object.getTexture())  // TODO(vis4rd): remove this branch when all textures are
                                     // initialized
        {
            m_renderer.drawQuad(
                map_object.getPosition(),
                map_object.getSize(),
                map_object.getRotation(),
                map_object.getTexture(),
                {0.9f, 0.9f, 1.f, 0.2f});
        }
    }
    else if(selectedMapItem > BlockID::FIRST_BLOCK && selectedMapItem < BlockID::LAST_BLOCK)
    {
        // hovered tile highlight
        m_renderer.drawQuad(
            {std::round(mouseWorldPos.x), std::round(mouseWorldPos.y)},
            {1.f, 1.f},
            0.f,
            {0.9f, 0.9f, 1.f, 0.2f});
    }
    else if(selectedMapItem == 9999)
    {
        m_renderer
            .drawRect({mouseWorldPos.x, mouseWorldPos.y}, endAreaSize, 0.f, {1.f, 1.f, 1.f, 1.f});
    }
    else if(selectedMapItem == 10000)
    {
        m_renderer.drawQuad(
            {mouseWorldPos.x, mouseWorldPos.y},
            {1.f, 1.f},
            randomizedRotation,
            ResourceManager::enemyTexture,
            {1.f, 0.4f, 0.4f, 0.2f});
    }

    auto view = m_entities.view<const ecs::component::Position, const ecs::component::Rotation>();
    for(auto&& [enemy, pos, rot] : view.each())
    {
        m_renderer.drawQuad(
            pos,
            {1.f, 1.f},
            rot.data,
            ResourceManager::enemyTexture,
            {1.f, 0.4f, 0.4f, 1.f});
    }
    // Renderer::drawRect({std::round(mouseWorldPos.x), std::round(mouseWorldPos.y)},
    // {1.f, 1.f}, 0.f, {1.f, 1.f, 1.f, 1.f}); Renderer::drawRect({-5.f, 5.f}, {5.f, 5.f}, {5.f,
    // -5.f}, {-5.f, -5.f}, {0.f, 1.f, 0.f, 1.f}); Renderer::drawRect({-3.f, 3.f}, {3.f, -3.f},
    // {0.f, 1.f, 0.f, 1.f}); Renderer::drawLine({0.f, 0.f}, {std::round(mouseWorldPos.x),
    // std::round(mouseWorldPos.y)}, {1.f, 0.f, 0.f, 1.f}); Renderer::drawLine({7.f, 7.f}, {7.f,
    // -7.f}, {0.f, 0.f, 1.f, 1.f}, {1.f, 1.f, 1.f, 1.f});
    if(draw_bbs)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    m_renderer.endBatch(m_camera.getProjectionMatrix(), m_camera.getViewMatrix());
    if(draw_bbs)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    const auto& camera_pos = m_camera.getPosition();
    const auto& camera_target = m_camera.getTargetPosition();
    const glm::vec2 mouse_screen_pos = ResourceManager::window->getMousePosition();
    mouseWorldPos = this->mouseScreenPosToWorldPos(mouse_screen_pos, m_camera);
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
        ImGui::Text(
            "Camera position: (%.2f, %.2f, %.2f)",
            camera_pos.x,
            camera_pos.y,
            camera_pos.z);
        ImGui::Text(
            "Camera target: (%.2f, %.2f, %.2f)",
            camera_target.x,
            camera_target.y,
            camera_target.z);
    }
    ImGui::End();

    ImGui::Begin("Section options");
    {
        const std::size_t i = std::llroundf(mouseWorldPos.x);
        const std::size_t j = std::llroundf(mouseWorldPos.y);
        ImGui::Text("Hovered tile: (%ld, %ld)", i, j);
        ImGui::Text("Mouse screen position: (%.2f, %.2f)", mouse_screen_pos.x, mouse_screen_pos.y);
        ImGui::Text("Mouse world position: (%.2f, %.2f)", mouseWorldPos.x, mouseWorldPos.y);
        ImGui::Text("Time since app start: %lf", ResourceManager::timer->getTotalTime());
        ImGui::Text("Time since last frame: %lf", ResourceManager::timer->deltaTime());

        ImGui::Separator();
        static std::string preview = "Wall";
        std::array<
            bool,
            static_cast<std::size_t>(BlockID::BLOCK_COUNT)
                + static_cast<std::size_t>(ObjectID::OBJECT_COUNT) + 1 + 1>
            checks = {false};
        if(ImGui::BeginCombo("Blocks", preview.c_str()))
        {
            for(std::size_t block_id = BlockID::FIRST_BLOCK + 1; block_id < BlockID::LAST_BLOCK;
                block_id++)
            {
                if(ImGui::Selectable(
                       (blockToString(block_id) + std::string("##unique_id")).c_str(),
                       &(checks.at(block_id - BlockID::FIRST_BLOCK - 1))))
                {
                    spdlog::debug("Selected Block '{}'", blockToString(block_id));
                    selectedMapItem = block_id;
                    preview = blockToString(block_id);
                }
            }
            for(std::size_t object_id = ObjectID::FIRST_OBJECT + 1;
                object_id < ObjectID::LAST_OBJECT;
                object_id++)
            {
                if(ImGui::Selectable(
                       (objectIdToString(object_id) + std::string("##unique_id")).c_str(),
                       &(checks.at(BlockID::BLOCK_COUNT + object_id - ObjectID::FIRST_OBJECT - 1))))
                {
                    spdlog::debug("Selected MapObject '{}'", objectIdToString(object_id));
                    selectedMapItem = object_id;
                    preview = objectIdToString(object_id);
                }
            }
            if(ImGui::Selectable(
                   "EndArea##unique_id",
                   &(checks
                         [static_cast<std::size_t>(BlockID::BLOCK_COUNT)
                          + static_cast<std::size_t>(ObjectID::OBJECT_COUNT)])))
            {
                spdlog::debug("Selected End Area");
                selectedMapItem = 9999;
                preview = "End Area";
            }
            if(ImGui::Selectable("Enemy##unique_id", &(checks.back())))
            {
                spdlog::debug("Selected Enemy");
                selectedMapItem = 10000;
                preview = "Enemy";
            }
            ImGui::EndCombo();
        }

        ImGui::Checkbox("Solid tile", &selectedSolid);
        ImGui::Checkbox("Draw map area", &draw_area);
        ImGui::Checkbox("Draw bounding boxes", &draw_bbs);
        if(ImGui::Button("Save to file"))
        {
            std::array<const char*, 1> patterns = {"*.msmap"};
            const char* file_path = tinyfd_saveFileDialog(
                "Choose save location...",
                "./",
                patterns.size(),
                patterns.data(),
                "*.msmap - Monoshot map savefile");
            if(nullptr != file_path)
            {
                m_map.saveToFile(file_path, m_entities);
            }
        }
        ImGui::SameLine();
        if(ImGui::Button("Load from file"))
        {
            std::array<const char*, 2> patterns = {"*.msmap", "*.map"};
            const char* file_path = tinyfd_openFileDialog(
                "Load a map...",
                "./",
                patterns.size(),
                patterns.data(),
                nullptr,
                0);
            if(nullptr != file_path)
            {
                m_map.loadFromFile(file_path, m_entities);
            }
        }

        ImGui::SliderFloat("Object rotation", &randomizedRotation, 0.f, 360.f);
    }
    ImGui::End();
}

// returns world coordinates at 0 height
glm::vec2 CreatorSection::mouseScreenPosToWorldPos(const glm::vec2& mouse_pos, Camera& camera)
{
    const auto& inverse_projection_matrix = camera.getInverseProjectionMatrix();
    const auto& inverse_view_matrix = camera.getInverseViewMatrix();

    const auto& mouse_x = mouse_pos.x;
    const auto& mouse_y = mouse_pos.y;
    const auto& window_size = ResourceManager::window->getSize();
    const auto& window_w = window_size.x;
    const auto& window_h = window_size.y;

    const float norm_mouse_x = (2.f * mouse_x / static_cast<float>(window_w)) - 1.f;
    const float norm_mouse_y = 1.f - (2.f * mouse_y / static_cast<float>(window_h));

    glm::vec3 norm_mouse_vector = glm::vec3(norm_mouse_x, norm_mouse_y, 1.f);
    glm::vec4 ray_clip = glm::vec4(norm_mouse_vector.x, norm_mouse_vector.y, -1.f, 1.f);
    glm::vec4 ray_eye = inverse_projection_matrix * ray_clip;
    ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.f, 0.f);
    glm::vec3 ray_world = glm::vec3((inverse_view_matrix * ray_eye));
    ray_world = glm::normalize(ray_world);

    float l = -(camera.getPosition().z / ray_world.z);

    return {camera.getPosition().x + l * ray_world.x, camera.getPosition().y + l * ray_world.y};
}
