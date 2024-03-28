#include "../../include/section/DebugSection.hpp"

#include <input/InputManager.hpp>
#include <renderer/Renderer.hpp>
#include <resource/ResourceManager.hpp>
#include <section/SectionManager.hpp>
#include <stbi/stb_image.h>

#include "../../include/ecs/actions.hpp"
#include "../../include/ecs/systems.hpp"
#include "../../include/ui/GameplayOverlay.hpp"
#include "../../include/utility/Collisions.hpp"

DebugSection::DebugSection()
    : Section()
    , m_camera(glm::vec3(0.f, 0.f, 50.f), ResourceManager::window->getSize())
    , m_map(5, 5)
    , m_hero(100)
    , m_layout(ImGui::GetMainViewport()->WorkPos, ImGui::GetMainViewport()->WorkSize)
{
    m_name = "DebugSection";

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
    // VBO data stored in pattern: [attr1_el0_data0 ... attr1_el0_datan, attr2_el0_data0 ...
    // attr2_el0_datan, attr1_el1_data, attr2_el1_data, ..., attr1_eln, attr2_eln] Example: v -
    // vertex; p - position; x, y, z - position axes; c - color; r, g, b, a - color channels
    //     vbo_data = [v1x, v1y, v1z, v1r, v1g, v1b, v1a,, v2p, v2c, v3p, v3c,, v4, ..., vn]

    m_map.setTile(5, 5, 0, BlockID::Wall, true);

    m_hero.addItem(std::move(Weapon(10, 31, 76, 35.f, 0.3)));
    m_hero.addItem(std::move(Weapon(10, 70, 20000, 10.f, 0.05)));

    // ecs
    m_map.addTilesToRegistry(m_mapElementsRegistry);
}

DebugSection::~DebugSection()
{
    spdlog::trace("Destroying DebugSection");
    m_mapElementsRegistry.clear();
}

void DebugSection::update() noexcept
{
    const glm::vec2& pos = m_hero.position;
    float& rot = m_hero.rotation;

    // calculate main hero rotation after mouse cursor
    const glm::vec2 mouse_screen_pos = ResourceManager::window->getMousePosition();
    const auto mouse_world_pos = this->mouseScreenPosToWorldPos(mouse_screen_pos, m_camera);
    rot = glm::degrees(std::atan2(mouse_world_pos.y - pos.y, mouse_world_pos.x - pos.x));

    // if(const bool is_during_entrance = this->onEnter(); is_during_entrance)
    // {
    //     return;
    // }
    if(const bool is_leave_finished = this->onLeave(); is_leave_finished)
    {
        SectionManager::get().popSection();
        return;
    }

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
    move_direction.x -= static_cast<float>(input.isHeld(GLFW_KEY_A) + input.isPressedOnce(GLFW_KEY_LEFT));
    move_direction.x += static_cast<float>(input.isHeld(GLFW_KEY_D) + input.isPressedOnce(GLFW_KEY_RIGHT));
    move_direction.y -= static_cast<float>(input.isHeld(GLFW_KEY_S) + input.isPressedOnce(GLFW_KEY_DOWN));
    move_direction.y += static_cast<float>(input.isHeld(GLFW_KEY_W) + input.isPressedOnce(GLFW_KEY_UP));
    {
        const bool does_move = static_cast<bool>((move_direction.x != 0.f) + (move_direction.y != 0.f));
        if(does_move)
        {
            m_hero.getTexture()->nextFrame();
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
                    ecs::action::spawnBullet(m_bulletRegistry, pos, rot, weapon.getBulletVelocity());
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
            m_hero.setCurrentItem(0);
        }
        if(input.isPressedOnce(GLFW_KEY_2))
        {
            m_hero.setCurrentItem(1);
        }
        if(input.isPressedOnce(GLFW_KEY_3))
        {
            m_hero.setCurrentItem(2);
        }
    }

    // make camera follow main hero
    m_camera.setPosition({pos, m_camera.getPosition().z});
    m_camera.setTarget({pos, 0.f});

    ecs::system::updateAis(m_enemyRegistry, m_hero.position, m_bulletRegistry);
    ecs::system::moveHeroWithCollisions(m_mapElementsRegistry, m_hero, move_direction);
    ecs::system::moveBullets(m_bulletRegistry);
    ecs::system::collideBullets(m_bulletRegistry, m_mapElementsRegistry, m_enemyRegistry);
    ecs::system::checkAliveBullets(m_bulletRegistry);
    ecs::system::destroyEntities(m_bulletRegistry);
    ecs::system::destroyEntities(m_enemyRegistry);

    // finish the level if hero gets to the end area
    if(m_map.isInEndArea(pos, m_hero.m_size) && (not m_onLeaveStarted))
    {
        m_leaveStartTimestamp = Timer::getTotalTime();
        m_onLeaveStarted = true;
    }
    // clang-format on
}

void DebugSection::render() noexcept
{
    spdlog::trace("Rendering DebugSection");

    // m_renderer.beginBatch();

    static bool draw_area = false;
    static bool draw_bounding_boxes = false;
    m_map.drawTiles(draw_area, draw_bounding_boxes);

    glm::vec2& pos = m_hero.position;
    float& rot = m_hero.rotation;
    float& vel = m_hero.velocity;
    const float& acc = m_hero.m_acceleration;
    const auto& theme_color = std::get<1>(m_map.getCurrentTheme().wallBlock);
    mono::renderer::drawQuad({pos.x, pos.y}, m_hero.m_size, rot, m_hero.getTexture(), theme_color);

    m_map.drawObjects({pos.x, pos.y}, draw_bounding_boxes);

    const auto bullet_view = m_bulletRegistry.view<
        const ecs::component::Position,
        const ecs::component::Size,
        const ecs::component::Rotation>(entt::exclude<ecs::component::Destroyed>);
    bullet_view.each([&theme_color](const auto& b_pos, const auto& b_size, const auto& b_rot) {
        mono::renderer::drawQuad({b_pos.x, b_pos.y}, b_size, b_rot, theme_color);
    });

    const auto& enemy_texture = ResourceManager::enemyTexture;
    const auto enemy_view = m_enemyRegistry.view<
        const ecs::component::Position,
        const ecs::component::Size,
        const ecs::component::Rotation>();
    enemy_view.each([&enemy_texture](const auto& e_pos, const auto& e_size, const auto& e_rot) {
        mono::renderer::drawQuad(
            {e_pos.x, e_pos.y},
            e_size,
            e_rot,
            enemy_texture,
            {1.f, 0.4f, 0.4f, 1.f});
    });

    mono::renderer::render(m_camera.getProjectionMatrix(), m_camera.getViewMatrix());

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
        UI::drawOverlay(
            m_layout,
            m_hero.health,
            m_hero.m_maxHealth,
            current_ammo,
            total_ammo,
            m_hero.getCurrentItemIndex());
    }
    if(m_onLeaveStarted)
    {
        const auto text_centered = [](const char* text, auto&&... args) {
            float font_size = ImGui::GetFontSize() * static_cast<float>(std::strlen(text)) / 2;
            ImGui::SameLine(ImGui::GetWindowSize().x / 2 - font_size + (font_size / 2));

            ImGui::Text("%s", text, args...);
        };
        using res = ResourceManager;
        const auto& font = res::uiTitleFont;
        auto font_guard = font->use();
        const auto text_pos = res::window->getSize() / 2;
        ImGui::SetNextWindowPos(
            {static_cast<float>(text_pos.x), static_cast<float>(text_pos.y)},
            ImGuiCond_Always,
            {0.5f, 0.5f});
        ImGui::SetNextWindowSize(
            {static_cast<float>(text_pos.x), static_cast<float>(text_pos.y) / 1.5f});
        ImGui::Begin("Win message", nullptr, m_layout.windowFlags | ImGuiWindowFlags_NoBackground);
        {
            text_centered("You won");
        }
        ImGui::End();
        font_guard.popFont();
    }

    this->showDebugUI(draw_area, draw_bounding_boxes);
}

// returns world coordinates at 0 height
glm::vec2 DebugSection::mouseScreenPosToWorldPos(const glm::vec2& mouse_pos, Camera& camera)
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
        m_camera.setPosition(
            {camera_pos.x, camera_pos.y, starting_zoom + (zoom_diff * (1.0 - diff_multiplier))});

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
        m_camera.setPosition(
            {camera_pos.x, camera_pos.y, starting_zoom + (zoom_diff * diff_multiplier)});

        return diff > m_leaveDuration;
    }

    return false;
}

void DebugSection::showDebugUI(bool& draw_area, bool& draw_bounding_boxes)
{
    const glm::vec2& pos = m_hero.position;
    float& rot = m_hero.rotation;
    float& vel = m_hero.velocity;
    const float& acc = m_hero.m_acceleration;

    ImGui::Begin("Section options");
    {
        static float zoom = 50.f;
        // ImGui::SliderFloat("rotation", &rotation, -360.f, 360.f, "%.0f degrees");
        // ImGui::SliderFloat2("scale", reinterpret_cast<float*>(&scale), 0.01f, 5.f);
        // ImGui::SliderFloat2("position", reinterpret_cast<float*>(&position), -10.f, 10.f);
        if(ImGui::SliderFloat("camera zoom", &zoom, 0.1f, 200.f, "x%.1f"))
        {
            const auto& camera_pos = m_camera.getPosition();
            m_camera.setPosition({camera_pos.x, camera_pos.y, zoom});
        }
        ImGui::Text(
            "Performance: [%.2fms] [%.0ffps]",
            1000.0f / ImGui::GetIO().Framerate,
            ImGui::GetIO().Framerate);
        ImGui::Text(
            "Mouse Position: Screen[%.2fx, %.2fy]",
            ImGui::GetMousePos().x,
            ImGui::GetMousePos().y);
        const glm::vec2 mouse_screen_pos = ResourceManager::window->getMousePosition();
        const auto mouse_world_pos = this->mouseScreenPosToWorldPos(mouse_screen_pos, m_camera);
        ImGui::Text("mouse screen position: (%f, %f)", mouse_screen_pos.x, mouse_screen_pos.y);
        ImGui::Text("mouse world position: (%f, %f)", mouse_world_pos.x, mouse_world_pos.y);
        ImGui::Text(
            "hero: pos(%.2f, %.2f), vel(%.2f), acc(%.2f), rot(%.2f)",
            pos.x,
            pos.y,
            vel,
            acc,
            rot);
        ImGui::Text("health: %d/%d", m_hero.health, m_hero.m_maxHealth);
        // const auto* weapon = dynamic_cast<Weapon*>(&*(m_hero.currentItem));
        if(!m_hero.isInventoryEmpty())
        {
            ImGui::Text("Selected item: %llu", m_hero.getCurrentItemIndex());
            if(m_hero.holdsWeapon())
            {
                const auto& weapon = m_hero.getCurrentItem<Weapon>();
                ImGui::Text(
                    "ammo: %u/%u/%u",
                    weapon.getAmmoCurrent(),
                    weapon.getAmmoMagazineMax(),
                    weapon.getAmmoTotal());
            }
        }
        ImGui::Separator();
        ImGui::Text(
            "Map elements count: %lld",
            m_mapElementsRegistry.storage<ecs::component::Position>().size());
        ImGui::Text(
            "Bullet count: %lld",
            m_bulletRegistry.storage<ecs::component::Position>().size());

        static std::string preview = "Forest Theme";
        bool check = false;
        if(ImGui::BeginCombo("map_theme", preview.c_str()))
        {
            if(ImGui::Selectable("Tutorial Theme##unique_id", &check))
            {
                preview = "Tutorial Theme";
                spdlog::debug("Switching MapTheme to '{}'", preview);
                m_map.setTheme(MapThemes::tutorialTheme);
            }
            if(ImGui::Selectable("Forest Theme##unique_id", &check))
            {
                preview = "Forest Theme";
                spdlog::debug("Switching MapTheme to '{}'", preview);
                m_map.setTheme(MapThemes::forestTheme);
            }
            if(ImGui::Selectable("Winter Theme##unique_id", &check))
            {
                preview = "Winter Theme";
                spdlog::debug("Switching MapTheme to '{}'", preview);
                m_map.setTheme(MapThemes::winterTheme);
            }
            ImGui::EndCombo();
        }
        ImGui::Checkbox("Draw map area", &draw_area);
        ImGui::Checkbox("Draw bounding boxes", &draw_bounding_boxes);

        auto& clear_color = ResourceManager::mapThemeBackgroundColor;
        if(ImGui::ColorEdit3("clear color", &(clear_color.x)))
        {
            glClearColor(
                clear_color.r * clear_color.a,
                clear_color.g * clear_color.a,
                clear_color.b * clear_color.a,
                clear_color.a);
        }
    }
    ImGui::End();
}
