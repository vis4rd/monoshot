#include "../../include/section/DebugSection.hpp"

#include "../../include/ui/elements/GameplayOverlay.hpp"
#include "../../include/renderer/Renderer.hpp"
#include "../../include/utility/ResourceManager.hpp"
#include "../../include/utility/Collisions.hpp"

#include <stbi/stb_image.h>

DebugSection::DebugSection()
    : Section(),
      // VAO(),
      m_camera(glm::vec3(0.f, 0.f, 50.f), ResourceManager::window->getSize()),
      m_map(5, 5),
      m_hero(100),
      m_layout(ImGui::GetMainViewport()->WorkPos, ImGui::GetMainViewport()->WorkSize)
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

    m_map.loadFromFile("testMap.map");

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

    m_hero.addItem(std::move(Weapon(10, 31, 76, 35.f, 0.3)));
    m_hero.addItem(std::move(Weapon(10, 70, 20000, 10.f, 0.05)));

    // ecs
    m_map.addTilesToRegistry(m_mapElementsRegistry);

    // sounds and music
    const auto setupSound = [&buffers = m_soundBuffers, &sounds = m_sounds](const std::string& name, const std::string& filename) -> void
    {
        sf::SoundBuffer buffer{};
        if(bool success = buffer.loadFromFile(filename); not success)
        {
            spdlog::error("Could not load sound from '{}' file", filename);
            throw std::runtime_error("Could not load sound from '" + filename + "' file");
        }
        buffers[name] = std::move(buffer);

        sf::Sound sound{};
        sound.setBuffer(buffers[name]);
        sounds[name] = std::move(sound);
    };
    setupSound("pop", "../res/audio/pop.mp3");
    setupSound("gunshot", "../res/audio/gunshot.mp3");
    setupSound("footstep", "../res/audio/footstep.mp3");
    setupSound("handgun_click", "../res/audio/handgun_click.mp3");

    if(bool success = m_music.openFromFile("../res/audio/music/Ancient Jungle Ruins - HeatleyBros.mp3"); not success)
    {
        spdlog::debug("Could not load music from file 'res/audio/music/Ancient Jungle Ruins - HeatleyBros.mp3'");
        throw std::runtime_error("Could not load music from file 'res/audio/music/Ancient Jungle Ruins - HeatleyBros.mp3'");
    }
    m_music.play();
}

DebugSection::~DebugSection()
{
    spdlog::trace("Destroying DebugSection");
    Renderer::shutdown();
    // firstTexture->destroy();
    m_mapElementsRegistry.clear();
}

void DebugSection::update() noexcept
{
    // spdlog::trace("Updating DebugSection");
    const glm::vec2& pos = m_hero.position;
    float& rot = m_hero.rotation;

    // update music
    // UpdateMusicStream(m_music);
    // m_music.update();

    // calculate main hero rotation after mouse cursor
    const glm::vec2 mouse_screen_pos = ResourceManager::window->getMousePosition();
    const auto mouse_world_pos = this->mouseScreenPosToWorldPos(mouse_screen_pos, m_camera);
    rot = glm::degrees(std::atan2(mouse_world_pos.y - pos.y, mouse_world_pos.x - pos.x));

    if(const bool is_during_entrance = this->onEnter(); is_during_entrance)
    {
        return;
    }
    if(const bool is_leave_finished = this->onLeave(); is_leave_finished)
    {
        SectionManager::get().popSection();
        return;
    }

    // model_matrix = glm::translate(glm::mat4(1.f), position);
    // model_matrix = glm::rotate(model_matrix, glm::radians(rotation), glm::vec3(0.f, 0.f, 1.f));
    // model_matrix = glm::scale(model_matrix, scale);

    auto& input = InputManager::get();
    if(input.isPressedOnce(GLFW_KEY_ESCAPE))
    {
        SectionManager::get().popSection();
        return;
    }

    m_layout.update(ImGui::GetMainViewport()->WorkPos, ImGui::GetMainViewport()->WorkSize);

    // clang-format off

    // poll next move events
    glm::vec2 move_direction = {0.f, 0.f};
    move_direction.x -= bool(input.isHeld(GLFW_KEY_A) + input.isPressedOnce(GLFW_KEY_LEFT));
    move_direction.x += bool(input.isHeld(GLFW_KEY_D) + input.isPressedOnce(GLFW_KEY_RIGHT));
    move_direction.y -= bool(input.isHeld(GLFW_KEY_S) + input.isPressedOnce(GLFW_KEY_DOWN));
    move_direction.y += bool(input.isHeld(GLFW_KEY_W) + input.isPressedOnce(GLFW_KEY_UP));
    {
        const bool does_move = bool((move_direction.x != 0.f) + (move_direction.y != 0.f));
        static std::size_t footstep_sound_trigger;
        if(does_move)
        {
            const std::int32_t is_next_frame = m_hero.getTexture()->nextFrame();
            footstep_sound_trigger = (footstep_sound_trigger + is_next_frame) % (m_hero.getTexture()->getTextureData().numberOfSubs / 2);
            if(footstep_sound_trigger == 0)
            {
                m_sounds["footstep"].play();
            }
        }
        else
        {
            m_hero.getTexture()->resetFrame();
        }
    }

    // update inventory logic
    if(!m_hero.isInventoryEmpty())
    {
        if(!ImGui::GetIO().WantCaptureMouse && input.isHeld(GLFW_MOUSE_BUTTON_LEFT))
        {
            auto& item = m_hero.getCurrentItem<Consumable>();
            if(const bool is_used = item.useDelayed(); m_hero.holdsWeapon() && is_used)
            {
                const auto& weapon = m_hero.getCurrentItem<Weapon>();
                if(weapon.getAmmoCurrent() > 0)
                {
                    auto bullet = m_bulletRegistry.create();
                    m_bulletRegistry.emplace<ecs::component::lifetime>(bullet, ResourceManager::timer->getTotalTime(), 2.0);
                    m_bulletRegistry.emplace<ecs::component::position>(bullet, pos);
                    m_bulletRegistry.emplace<ecs::component::size>(bullet, glm::vec2{0.1f, 0.1f});
                    m_bulletRegistry.emplace<ecs::component::velocity>(bullet, weapon.getBulletVelocity());
                    m_bulletRegistry.emplace<ecs::component::max_velocity>(bullet, weapon.getBulletVelocity());
                    m_bulletRegistry.emplace<ecs::component::acceleration>(bullet, -1.f);
                    m_bulletRegistry.emplace<ecs::component::rotation>(bullet, rot);
                    m_sounds["gunshot"].play();
                }
            }
        }
        if(m_hero.holdsWeapon() && input.isHeld(GLFW_KEY_R))
        {
            auto& weapon = m_hero.getCurrentItem<Weapon>();
            weapon.reload();
        }
        if(input.isPressedOnce(GLFW_KEY_G))
        {
            m_hero.dropCurrentItem();
        }
        if(input.isPressedOnce(GLFW_KEY_1))
        {
            m_sounds["handgun_click"].play();
            m_hero.setCurrentItem(0);
        }
        if(input.isPressedOnce(GLFW_KEY_2))
        {
            m_sounds["handgun_click"].play();
            m_hero.setCurrentItem(1);
        }
        if(input.isPressedOnce(GLFW_KEY_3))
        {
            m_sounds["handgun_click"].play();
            m_hero.setCurrentItem(2);
        }
    }

    // make camera follow main hero
    m_camera.setPosition({pos, m_camera.getPosition().z});
    m_camera.setTarget({pos, 0.f});

    // ecs::system::remove_dead_entities(m_registry);
    ecs::system::move_hero_with_collisions(m_mapElementsRegistry, m_hero, move_direction);
    ecs::system::move_bullets(m_bulletRegistry);
    ecs::system::collide_bullets(m_bulletRegistry, m_mapElementsRegistry);
    ecs::system::check_alive_bullets(m_bulletRegistry);
    ecs::system::destroy_bullets(m_bulletRegistry);

    // finish the level if hero gets to the end area
    if(m_map.isInEndArea(pos, m_hero.size) && (not m_onLeaveStarted))
    {
        m_leaveStartTimestamp = Timer::getTotalTime();
        m_onLeaveStarted = true;
    }
    // clang-format on
}

static bool s_draw_area = false;
static bool s_draw_bbs = false;

void DebugSection::render() noexcept
{
    spdlog::trace("Rendering DebugSection");

    Renderer::beginBatch();
    m_map.drawTiles(s_draw_area, s_draw_bbs);

    // Renderer::drawQuad({0.f, 10.f}, tile_size, 0.f, {1.f, 0.5f, 0.5f, 1.f});
    // Renderer::drawQuad({0.f, 8.f}, tile_size, 0.f, {1.f, 0.5f, 0.5f, 1.f});
    // Renderer::drawQuad({9.f, 12.f}, tile_size, 45.f, {1.f, 0.5f, 0.5f, 1.f});
    // Renderer::drawQuad({1.f, -1.f}, tile_size, 45.f, firstTexture, {1.f, 1.f, 1.f, 1.f});
    // Renderer::drawQuad({-46.f, 0.f}, tile_size, 90.f, firstTexture, {1.f, 1.f, 1.f, 1.f});

    glm::vec2& pos = m_hero.position;
    float& rot = m_hero.rotation;
    float& vel = m_hero.velocity;
    const float& acc = m_hero.acceleration;
    const auto& theme_color = std::get<1>(m_map.getCurrentTheme().wallBlock);
    Renderer::drawQuad({pos.x, pos.y}, m_hero.size, rot, m_hero.getTexture(), theme_color);

    m_map.drawObjects({pos.x, pos.y}, s_draw_bbs);

    const auto bullet_view = m_bulletRegistry.view<const ecs::component::position, const ecs::component::size, const ecs::component::rotation>(entt::exclude<ecs::component::destroyed>);
    bullet_view.each(
        [&theme_color](const auto& b_pos, const auto& b_size, const auto& b_rot)
        {
            Renderer::drawQuad({b_pos.x, b_pos.y}, b_size, b_rot, theme_color);
        });

    Renderer::endBatch(m_camera.getProjectionMatrix(), m_camera.getViewMatrix());

    // auto& triangle_zoom_shader = ShaderManager::useShader("triangle_zoom");
    // triangle_zoom_shader.uploadMat4("uTransform", model_matrix, 0);
    // triangle_zoom_shader.uploadMat4("uProjection", m_camera.getProjectionMatrix(), 1);
    // triangle_zoom_shader.uploadMat4("uView", m_camera.getViewMatrix(), 2);
    // VAO.bind();
    // glDrawElements(GL_TRIANGLES, VAO.getElementBuffer().getElementCount(), GL_UNSIGNED_INT, 0);

    if(m_onEnterFinished && (not m_onLeaveStarted))
    {
        std::uint32_t current_ammo = 1;
        std::uint32_t total_ammo = 0;
        if(m_hero.holdsWeapon())
        {
            const auto& weapon = m_hero.getCurrentItem<Weapon>();
            current_ammo = weapon.getAmmoCurrent();
            total_ammo = weapon.getAmmoTotal();
        }
        UI::drawOverlay(m_layout, m_hero.health, m_hero.maxHealth, current_ammo, total_ammo, m_hero.getCurrentItemIndex());
    }
    if(m_onLeaveStarted)
    {
        const auto TextCentered = [](const char* text, auto&&... args)
        {
            float font_size = ImGui::GetFontSize() * strlen(text) / 2;
            ImGui::SameLine(ImGui::GetWindowSize().x / 2 - font_size + (font_size / 2));

            ImGui::Text("%s", text, args...);
        };
        using res = ResourceManager;
        const auto& font = res::uiTitleFont;
        auto font_guard = font->use();
        const auto text_pos = res::window->getSize() / 2;
        ImGui::SetNextWindowPos({static_cast<float>(text_pos.x), static_cast<float>(text_pos.y)}, ImGuiCond_Always, {0.5f, 0.5f});
        ImGui::SetNextWindowSize({static_cast<float>(text_pos.x), static_cast<float>(text_pos.y) / 1.5f});
        ImGui::Begin("Win message", nullptr, m_layout.window_flags | ImGuiWindowFlags_NoBackground);
        {
            TextCentered("You won");
        }
        ImGui::End();
        font_guard.popFont();
    }

    this->showDebugUI();
}

// returns world coordinates at 0 height
glm::vec2 DebugSection::mouseScreenPosToWorldPos(const glm::vec2& mouse_pos, Camera& camera)
{
    const auto& inverse_projection_matrix = camera.getInverseProjectionMatrix();
    const auto& inverse_view_matrix = camera.getInverseViewMatrix();
    // spdlog::trace("Inverse projection matrix = {}", util::mat4str(inverse_projection_matrix));
    // spdlog::trace("Inverse view matrix = {}", util::mat4str(inverse_view_matrix));

    const auto& mouse_x = mouse_pos.x;
    const auto& mouse_y = mouse_pos.y;
    const auto& window_size = ResourceManager::window->getSize();
    const auto& window_w = window_size.x;
    const auto& window_h = window_size.y;
    // spdlog::trace("Window size = ({}, {})", window_w, window_h);

    const float norm_mouse_x = (2.f * mouse_x / window_w) - 1.f;
    const float norm_mouse_y = 1.f - (2.f * mouse_y / window_h);
    // spdlog::trace("Normalized mouse position = ({}, {})", norm_mouse_x, norm_mouse_y);

    glm::vec3 norm_mouse_vector = glm::vec3(norm_mouse_x, norm_mouse_y, 1.f);
    glm::vec4 ray_clip = glm::vec4(norm_mouse_vector.x, norm_mouse_vector.y, -1.f, 1.f);
    glm::vec4 ray_eye = inverse_projection_matrix * ray_clip;
    ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.f, 0.f);
    glm::vec3 ray_world = glm::vec3((inverse_view_matrix * ray_eye));
    ray_world = glm::normalize(ray_world);
    // spdlog::trace("Ray world = {}", util::vec3str(ray_world));

    float l = -(camera.getPosition().z / ray_world.z);
    // spdlog::trace("L = {}", l);

    return {camera.getPosition().x + l * ray_world.x, camera.getPosition().y + l * ray_world.y};
}

/**
 * @brief Function executed when entering this section
 *
 * @return true when entrance sequence is not finished, false otherwise
 */
bool DebugSection::onEnter()
{
    if(not m_onEnterFinished)
    {
        const double current_time = Timer::getTotalTime();
        const double diff = m_enterFinishTimestamp - current_time;
        const double diff_multiplier = diff / m_entranceDuration;

        const float starting_zoom = 50.f;
        const float target_zoom = 20.f;
        const float zoom_diff = target_zoom - starting_zoom;

        const auto& camera_pos = m_camera.getPosition();
        m_camera.setPosition({camera_pos.x, camera_pos.y, starting_zoom + (zoom_diff * (1.0 - diff_multiplier))});

        if(diff <= 0.0)
        {
            m_onEnterFinished = true;
            m_camera.setPosition({camera_pos.x, camera_pos.y, target_zoom});
        }
        return true;
    }
    return false;
}

/**
 * @brief Function executed when leaving this section
 *
 * @return true when leave sequence is not finished, false otherwise
 */
bool DebugSection::onLeave()
{
    if(m_onLeaveStarted)
    {
        const double current_time = Timer::getTotalTime();
        const double diff = current_time - m_leaveStartTimestamp;
        const double diff_multiplier = diff / m_leaveDuration;

        const float starting_zoom = 20.f;
        const float target_zoom = 50.f;
        const float zoom_diff = target_zoom - starting_zoom;

        const auto& camera_pos = m_camera.getPosition();
        m_camera.setPosition({camera_pos.x, camera_pos.y, starting_zoom + (zoom_diff * diff_multiplier)});

        return diff > m_leaveDuration;
    }

    return false;
}

void DebugSection::showDebugUI()
{
    if constexpr(Flag::DebugMode)
    {
        const glm::vec2& pos = m_hero.position;
        float& rot = m_hero.rotation;
        float& vel = m_hero.velocity;
        const float& acc = m_hero.acceleration;

        ImGui::Begin("Section options");
        {
            static float zoom = 50.f;
            // ImGui::SliderFloat("rotation", &rotation, -360.f, 360.f, "%.0f degrees");
            // ImGui::SliderFloat2("scale", reinterpret_cast<float*>(&scale), 0.01f, 5.f);
            // ImGui::SliderFloat2("position", reinterpret_cast<float*>(&position), -10.f, 10.f);
            if(ImGui::SliderFloat("camera zoom", &zoom, 0.1f, 200.f, "x%.1f"))
            {
                // m_camera.setZoom(zoom);
                const auto& camera_pos = m_camera.getPosition();
                m_camera.setPosition({camera_pos.x, camera_pos.y, zoom});
            }
            const glm::vec2 mouse_screen_pos = ResourceManager::window->getMousePosition();
            const auto mouse_world_pos = this->mouseScreenPosToWorldPos(mouse_screen_pos, m_camera);
            ImGui::Text("mouse screen position: (%f, %f)", mouse_screen_pos.x, mouse_screen_pos.y);
            ImGui::Text("mouse world position: (%f, %f)", mouse_world_pos.x, mouse_world_pos.y);
            ImGui::Text("hero: pos(%.2f, %.2f), vel(%.2f), acc(%.2f), rot(%.2f)", pos.x, pos.y, vel, acc, rot);
            ImGui::Text("health: %d/%d", m_hero.health, m_hero.maxHealth);
            // const auto* weapon = dynamic_cast<Weapon*>(&*(m_hero.currentItem));
            if(!m_hero.isInventoryEmpty())
            {
                ImGui::Text("Selected item: %lu", m_hero.getCurrentItemIndex());
                if(m_hero.holdsWeapon())
                {
                    const auto& weapon = m_hero.getCurrentItem<Weapon>();
                    ImGui::Text("ammo: %u/%u/%u", weapon.getAmmoCurrent(), weapon.getAmmoMagazineMax(), weapon.getAmmoTotal());
                }
            }
            ImGui::Separator();
            ImGui::Text("Map elements count: %ld/%ld", m_mapElementsRegistry.alive(), m_mapElementsRegistry.size());
            ImGui::Text("Bullet count: %ld/%ld", m_bulletRegistry.alive(), m_bulletRegistry.size());

            static std::string preview = "Forest Theme";
            bool check = false;
            if(ImGui::BeginCombo("map_theme", preview.c_str()))
            {
                if(ImGui::Selectable("Tutorial Theme##unique_id", &check))
                {
                    preview = "Tutorial Theme";
                    spdlog::debug("Switching MapTheme to '{}'", preview);
                    m_map.setTheme(MapThemes::TUTORIAL_THEME);
                }
                if(ImGui::Selectable("Forest Theme##unique_id", &check))
                {
                    preview = "Forest Theme";
                    spdlog::debug("Switching MapTheme to '{}'", preview);
                    m_map.setTheme(MapThemes::FOREST_THEME);
                }
                if(ImGui::Selectable("Winter Theme##unique_id", &check))
                {
                    preview = "Winter Theme";
                    spdlog::debug("Switching MapTheme to '{}'", preview);
                    m_map.setTheme(MapThemes::WINTER_THEME);
                }
                ImGui::EndCombo();
            }
            ImGui::Checkbox("Draw map area", &s_draw_area);
            ImGui::Checkbox("Draw bounding boxes", &s_draw_bbs);

            auto& clear_color = ResourceManager::mapThemeBackgroundColor;
            float* cc = reinterpret_cast<float*>(&clear_color);
            if(ImGui::ColorEdit3("clear color", cc))
            {
                glClearColor(clear_color.r * clear_color.a, clear_color.g * clear_color.a, clear_color.b * clear_color.a, clear_color.a);
            }
        }
        ImGui::End();
    }
    else
    {
        ImGui::Begin("Release Mode Statistics");
        {
            ImGui::Text("Performance: [%.2fms] [%.0ffps]", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::Text("Mouse Position: Screen[%.2fx, %.2fy]", ImGui::GetMousePos().x, ImGui::GetMousePos().y);
        }
        ImGui::End();
    }
}
