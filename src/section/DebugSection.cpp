#include "../../include/section/DebugSection.hpp"

#include "../../include/renderer/Renderer.hpp"
#include "../../include/utility/ResourceManager.hpp"
#include "../../include/ecs/systems.hpp"
#include "../../include/utility/Collisions.hpp"

#include <stb_image.h>

constexpr glm::vec2 hero_size = {0.6f, 0.6f};
constexpr glm::vec2 tile_size = {1.f, 1.f};

DebugSection::DebugSection()
    : Section(),
      // VAO(),
      m_camera(glm::vec3(0.f, 0.f, 50.f), ResourceManager::window->getSize()),
      m_mapGrid(5, 5),
      m_registry(),
      m_hero(m_registry.create())
{
    m_name = "DebugSection";

    // float colors[] = {1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f, 1.f, 0.f, 0.f, 1.f, 1.f, 0.5f, 0.5f, 0.5f, 1.f};
    // float vertices[12] = {-0.5f, -0.5f, 0.f, 0.5f, -0.5f, 0.f, 0.5f, 0.5f, 0.f, -0.5f, 0.5f, 0.f};
    // uint32_t indices[6] = {0, 1, 2, 2, 3, 0};

    // ElementBuffer EBO(indices, 6);

    // VertexBuffer VBO1(sizeof(vertices));
    // BufferLayout layout({BufferElement(ShaderDataType::float3, "aPos")});
    // VBO1.setLayout(layout);
    // VBO1.setData(vertices, sizeof(vertices));

    // VertexBuffer VBO2(sizeof(colors));
    // BufferLayout layout2({BufferElement(ShaderDataType::float4, "aColor")});
    // VBO2.setLayout(layout2);
    // VBO2.setData(colors, sizeof(colors));

    // VAO.addVertexBuffer(std::move(VBO1));
    // VAO.addVertexBuffer(std::move(VBO2));
    // VAO.addElementBuffer(EBO);

    // So the structure looks more or less like this:
    // VAO
    //  L___VBO 1
    //       L___attribute 1
    //       L___attribute 2
    //       L___attribute ...
    //       L___attribute n
    //  L___VBO 2
    //       L___n attributes
    //  L___VBO ...
    //  L___VBO n
    //
    //
    // For position and color we can achieve it in two ways:
    // 1) One VAO, two VBOs with one attribute each
    // First VBO: one attribute for position
    // Second VBO: one attribute for color
    //
    // 2) One VAO, one VBO with two attributes
    // VBO data stored in pattern: [attr1_el0_data0 ... attr1_el0_datan, attr2_el0_data0 ... attr2_el0_datan, attr1_el1_data, attr2_el1_data, ..., attr1_eln, attr2_eln]
    // Example: v - vertex; p - position; x, y, z - position axes; c - color; r, g, b, a - color channels
    //     vbo_data = [v1x, v1y, v1z, v1r, v1g, v1b, v1a,, v2p, v2c, v3p, v3c,, v4, ..., vn]


    // ShaderManager::addShaderProgram("../res/shaders", "triangle_zoom");

    m_mapGrid.loadFromFile("testMap.map");

    // firstTexture = std::make_shared<Texture2D>(16, 16);
    // firstTexture->load("../res/textures/first_texture.png");

    // glm::vec3 first = {vertices[0], vertices[1], vertices[2]};
    // glm::vec3 second = {vertices[3], vertices[4], vertices[5]};
    // glm::vec3 third = {vertices[6], vertices[7], vertices[8]};
    // glm::vec3 fourth = {vertices[9], vertices[10], vertices[11]};
    // spdlog::debug("Triangle:");
    // spdlog::debug("Object space: {}, {}, {}, {}", util::vec3str(first), util::vec3str(second), util::vec3str(third), util::vec3str(fourth));
    // first = model_matrix * glm::vec4(first, 1.f);
    // second = model_matrix * glm::vec4(second, 1.f);
    // third = model_matrix * glm::vec4(third, 1.f);
    // fourth = model_matrix * glm::vec4(fourth, 1.f);
    // spdlog::debug("World space: {}, {}, {}, {}", util::vec3str(first), util::vec3str(second), util::vec3str(third), util::vec3str(fourth));
    // first = m_camera.getViewMatrix() * glm::vec4(first, 1.f);
    // second = m_camera.getViewMatrix() * glm::vec4(second, 1.f);
    // third = m_camera.getViewMatrix() * glm::vec4(third, 1.f);
    // fourth = m_camera.getViewMatrix() * glm::vec4(fourth, 1.f);
    // spdlog::debug("Camera space: {}, {}, {}, {}", util::vec3str(first), util::vec3str(second), util::vec3str(third), util::vec3str(fourth));
    // first = m_camera.getProjectionMatrix() * glm::vec4(first, 1.f);
    // second = m_camera.getProjectionMatrix() * glm::vec4(second, 1.f);
    // third = m_camera.getProjectionMatrix() * glm::vec4(third, 1.f);
    // fourth = m_camera.getProjectionMatrix() * glm::vec4(fourth, 1.f);
    // spdlog::debug("Screen space: {}, {}, {}, {}", util::vec3str(first), util::vec3str(second), util::vec3str(third), util::vec3str(fourth));

    Renderer::init();

    // ecs
    m_registry.emplace<ecs::component::position>(m_hero);
    m_registry.emplace<ecs::component::velocity>(m_hero);
    m_registry.emplace<ecs::component::max_velocity>(m_hero);
    m_registry.emplace<ecs::component::acceleration>(m_hero);
    m_registry.emplace<ecs::component::rotation>(m_hero);
    m_registry.emplace<ecs::component::direction>(m_hero);

    m_mapGrid.addTilesToRegistry(m_registry);
}

DebugSection::~DebugSection()
{
    spdlog::trace("Destroying DebugSection");
    Renderer::shutdown();
    // firstTexture->destroy();
    m_registry.clear();
}

void DebugSection::update() noexcept
{
    spdlog::trace("Updating DebugSection");
    // model_matrix = glm::translate(glm::mat4(1.f), position);
    // model_matrix = glm::rotate(model_matrix, glm::radians(rotation), glm::vec3(0.f, 0.f, 1.f));
    // model_matrix = glm::scale(model_matrix, scale);

    auto& input = InputManager::get();
    if(input.isPressedOnce(GLFW_KEY_ESCAPE))
    {
        SectionManager::get().popSection();
    }

    // process main hero entity
    if(m_registry.valid(m_hero))
    {
        // clang-format off
        auto& delta_time = ResourceManager::timer->deltaTime();
        glm::vec2& pos = m_registry.get<ecs::component::position>(m_hero);
        float& vel = m_registry.get<ecs::component::velocity>(m_hero);
        float& mvel = m_registry.get<ecs::component::max_velocity>(m_hero);
        float& acc = m_registry.get<ecs::component::acceleration>(m_hero);
        glm::vec2& dir = m_registry.get<ecs::component::direction>(m_hero);
        float& rot = m_registry.get<ecs::component::rotation>(m_hero);

        // sort map tiles by distance to the hero
        m_registry.sort<ecs::component::position>([&pos](const auto& lhs, const auto& rhs)
        {
            return glm::abs(glm::length(lhs-pos)) < glm::abs(glm::length(rhs-pos));
        });
        
        // poll next move events
        glm::vec2 move_direction = {0.f, 0.f};
        if(input.isHeld(GLFW_KEY_A) || input.isPressedOnce(GLFW_KEY_LEFT)) { move_direction.x -= 1.f; }
        if(input.isHeld(GLFW_KEY_D) || input.isPressedOnce(GLFW_KEY_RIGHT)) { move_direction.x += 1.f; }
        if(input.isHeld(GLFW_KEY_W) || input.isPressedOnce(GLFW_KEY_UP)) { move_direction.y += 1.f; }
        if(input.isHeld(GLFW_KEY_S) || input.isPressedOnce(GLFW_KEY_DOWN)) { move_direction.y -= 1.f; }

        if(move_direction != glm::vec2(0.f, 0.f))
        {
            // calculate velocity and next position
            auto length = glm::length(move_direction);
            if(length > 0.f)
            {
                dir = move_direction;

                vel += acc * delta_time;
                if(vel > mvel)
                {
                    vel = mvel;
                }
            }
            auto next_pos = pos + vel * dir * static_cast<float>(delta_time);

            // forbid moving into solid tiles
            const auto view = m_registry.view<ecs::component::position>();
            bool is_collision = false;
            const auto signum = [](const float& val) -> float
            {
                if(val < 0.f)
                {
                    return -1.f;
                }
                else if(val > 0.f)
                {
                    return 1.f;
                }
                else
                {
                    return 0.f;
                }
            };
            for(std::int32_t iter = 0; const auto& tile : view)
            {
                if(tile == m_hero)
                {
                    continue;
                }
                if(iter >= 4)
                {
                    break;
                }
                const auto& tile_pos = view.get<ecs::component::position>(tile);
                const auto& tile_rot = 0.f;

                const auto intersection = AABB::findCollision(next_pos, hero_size, tile_pos, tile_size);
                if(intersection.x > 0.f && intersection.y > 0.f)
                {
                    glm::vec2 center_diff = glm::abs(tile_pos - pos);
                    glm::vec2 sum_of_sizes = hero_size + tile_size;
                    glm::vec2 ncd = center_diff / (sum_of_sizes / 2.f);  // normalized center difference
                    // glm::vec2 nss = glm::normalize(sum_of_sizes);  // normalized sum of sizes

                    if(center_diff.x * std::abs(ncd.x) > center_diff.y * std::abs(ncd.y))  // collision is horizontal
                    //? consider multiplying both by nss.x and nss.y
                    {
                        next_pos.x = next_pos.x - intersection.x * signum(dir.x);
                        // next_pos.y = pos.y;  // TODO: cut by the factor of how much shorter is the x movement
                                                //? (this is probably the cause of this stuttering)
                                                // currently it is not really necessary as incorrect offset values are truly minimal
                        dir.x = 0.f;
                    }
                    else  // collision is vertical
                    {
                        next_pos.y = next_pos.y - intersection.y * signum(dir.y);
                        // next_pos.x = pos.x;  // TODO: cut by the factor of how much shorter is the y movement
                                                //? (this is probably the cause of this stuttering)
                                                // currently it is not really necessary as incorrect offset values are truly minimal
                        dir.y = 0.f;
                    }
                }
                
                iter++;
            }
            
            pos = next_pos;

            // make camera follow main hero
            m_camera.setPosition({pos, m_camera.getPosition().z});
            m_camera.setTarget({pos, 0.f});
        }
        else
        {
            vel -= acc * delta_time;
            if(vel < 0.f)
            {
                vel = 0.f;
            }
        }

        // calculate main hero rotation after mouse cursor
        const glm::vec2 mouse_screen_pos = ResourceManager::window->getMousePosition();
        const auto mouse_world_pos = this->mouseScreenPosToWorldPos(mouse_screen_pos, m_camera);
        rot = glm::degrees(std::atan2(mouse_world_pos.y - pos.y, mouse_world_pos.x - pos.x));

        // clang-format on
    }
}

void DebugSection::render() noexcept
{
    spdlog::trace("Rendering DebugSection");

    Renderer::beginBatch();
    m_mapGrid.drawTiles();

    // Renderer::drawQuad({0.f, 10.f}, tile_size, 0.f, {1.f, 0.5f, 0.5f, 1.f});
    // Renderer::drawQuad({0.f, 8.f}, tile_size, 0.f, {1.f, 0.5f, 0.5f, 1.f});
    // Renderer::drawQuad({9.f, 12.f}, tile_size, 45.f, {1.f, 0.5f, 0.5f, 1.f});
    // Renderer::drawQuad({1.f, -1.f}, tile_size, 45.f, firstTexture, {1.f, 1.f, 1.f, 1.f});
    // Renderer::drawQuad({-46.f, 0.f}, tile_size, 90.f, firstTexture, {1.f, 1.f, 1.f, 1.f});

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    const auto& [pos, rot, vel, acc] = m_registry.get<ecs::component::position, ecs::component::rotation, ecs::component::velocity, ecs::component::acceleration>(m_hero);
    Renderer::drawQuad({pos.x, pos.y}, hero_size, rot, {1.f, 0.f, 0.f, 1.f});

    m_mapGrid.drawObjects({pos.x, pos.y});
    Renderer::endBatch();
    ShaderManager::getShader("quad").uploadMat4("uProjection", m_camera.getProjectionMatrix(), 0);
    ShaderManager::getShader("quad").uploadMat4("uView", m_camera.getViewMatrix(), 1);

    glDisable(GL_BLEND);

    // auto& triangle_zoom_shader = ShaderManager::useShader("triangle_zoom");
    // triangle_zoom_shader.uploadMat4("uTransform", model_matrix, 0);
    // triangle_zoom_shader.uploadMat4("uProjection", m_camera.getProjectionMatrix(), 1);
    // triangle_zoom_shader.uploadMat4("uView", m_camera.getViewMatrix(), 2);
    // VAO.bind();
    // glDrawElements(GL_TRIANGLES, VAO.getElementBuffer().getElementCount(), GL_UNSIGNED_INT, 0);

    if constexpr(Flag::DebugMode)
    {
        ImGui::Begin("Section options");
        {
            static float zoom = 50.f;
            // ImGui::SliderFloat("rotation", &rotation, -360.f, 360.f, "%.0f degrees");
            // ImGui::SliderFloat2("scale", reinterpret_cast<float*>(&scale), 0.01f, 5.f);
            // ImGui::SliderFloat2("position", reinterpret_cast<float*>(&position), -10.f, 10.f);
            if(ImGui::SliderFloat("camera zoom", &zoom, 0.1f, 200.f, "x%.1f"))
            {
                // m_camera.setZoom(zoom);
                const auto& pos = m_camera.getPosition();
                m_camera.setPosition({pos.x, pos.y, zoom});
            }
            const glm::vec2 mouse_screen_pos = ResourceManager::window->getMousePosition();
            const auto mouse_world_pos = this->mouseScreenPosToWorldPos(mouse_screen_pos, m_camera);
            ImGui::Text("mouse screen position: (%f, %f)", mouse_screen_pos.x, mouse_screen_pos.y);
            ImGui::Text("mouse world position: (%f, %f)", mouse_world_pos.x, mouse_world_pos.y);
            ImGui::Text("hero: pos(%.2f, %.2f), vel(%.2f), acc(%.2f), rot(%.2f)", pos.x, pos.y, vel.data, acc.data, rot.data);

            static std::string preview = "Forest Theme";
            bool check = false;
            if(ImGui::BeginCombo("map_theme", preview.c_str()))
            {
                if(ImGui::Selectable("Tutorial Theme##unique_id", &check))
                {
                    preview = "Tutorial Theme";
                    spdlog::debug("Switching MapTheme to '{}'", preview);
                    m_mapGrid.setTheme(MapThemes::TUTORIAL_THEME);
                }
                if(ImGui::Selectable("Forest Theme##unique_id", &check))
                {
                    preview = "Forest Theme";
                    spdlog::debug("Switching MapTheme to '{}'", preview);
                    m_mapGrid.setTheme(MapThemes::FOREST_THEME);
                }
                if(ImGui::Selectable("Winter Theme##unique_id", &check))
                {
                    preview = "Winter Theme";
                    spdlog::debug("Switching MapTheme to '{}'", preview);
                    m_mapGrid.setTheme(MapThemes::WINTER_THEME);
                }
                ImGui::EndCombo();
            }

            auto& clear_color = ResourceManager::mapThemeBackgroundColor;
            float* cc = reinterpret_cast<float*>(&clear_color);
            if(ImGui::ColorEdit3("clear color", cc))
            {
                glClearColor(clear_color.r * clear_color.a, clear_color.g * clear_color.a, clear_color.b * clear_color.a, clear_color.a);
            }
        }
        ImGui::End();
    }
}

// returns world coordinates at 0 height
glm::vec2 DebugSection::mouseScreenPosToWorldPos(const glm::vec2& mouse_pos, Camera& camera)
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
