#include "../../include/section/GameplaySection.hpp"

#include <renderer/Renderer.hpp>
#include <resource/ResourceManager.hpp>
#include <stbi/stb_image.h>

#include "../../include/ecs/actions.hpp"
#include "../../include/ecs/systems.hpp"
#include "../../include/ui/GameplayOverlay.hpp"

static bool s_show_debug_info = false;

GameplaySection::GameplaySection()
    : Section()
    , m_camera(glm::vec3(0.f, 0.f, 50.f), ResourceManager::window->getSize())
    , m_renderer()
    , m_map(m_renderer, 5, 5)
    , m_hero(100)
    , m_layout(ImGui::GetMainViewport()->WorkPos, ImGui::GetMainViewport()->WorkSize)
{
    m_name = "GameplaySection";
}

GameplaySection::~GameplaySection()
{
    spdlog::trace("Destroying {}", m_name);
    m_mapElementsRegistry.clear();
}

void GameplaySection::update() noexcept
{
    const glm::vec2& pos = m_hero.position;
    float& rot = m_hero.rotation;

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

    // press escape to leave
    auto& input = InputManager::get();
    if(input.isPressedOnce(GLFW_KEY_ESCAPE))
    {
        SectionManager::get().popSection();
        return;
    }
    if(input.isPressedOnce(GLFW_KEY_F10))
    {
        s_show_debug_info = !s_show_debug_info;
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
        if(does_move)
        {
            const std::int32_t is_next_frame = m_hero.getTexture()->nextFrame();
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

    // update dynamic logic
    ecs::system::update_ais(m_enemyRegistry, m_hero.position, m_bulletRegistry);
    ecs::system::move_hero_with_collisions(m_mapElementsRegistry, m_hero, move_direction);
    ecs::system::move_bullets(m_bulletRegistry);
    ecs::system::collide_bullets(m_bulletRegistry, m_mapElementsRegistry, m_enemyRegistry);
    ecs::system::check_alive_bullets(m_bulletRegistry);
    ecs::system::destroy_entities(m_bulletRegistry);
    ecs::system::destroy_entities(m_enemyRegistry);

    // finish the level if hero gets to the end area
    if(m_map.isInEndArea(pos, m_hero.size) && (not m_onLeaveStarted))
    {
        m_leaveStartTimestamp = Timer::getTotalTime();
        m_onLeaveStarted = true;
    }
    // clang-format on
}

void GameplaySection::render() noexcept
{
    spdlog::trace("Rendering GameplaySection");

    m_renderer.beginBatch();
    m_map.drawTiles();

    const glm::vec2& pos = m_hero.position;
    const float& rot = m_hero.rotation;
    const float& vel = m_hero.velocity;
    const float& acc = m_hero.acceleration;
    const auto& theme_color = std::get<1>(m_map.getCurrentTheme().wallBlock);
    m_renderer.drawQuad({pos.x, pos.y}, m_hero.size, rot, m_hero.getTexture(), theme_color);

    const auto& enemy_texture = ResourceManager::enemyTexture;
    const auto enemy_view = m_enemyRegistry.view<
        const ecs::component::position,
        const ecs::component::size,
        const ecs::component::rotation>();
    enemy_view.each(
        [&enemy_texture,
         &m_renderer = m_renderer](const auto& e_pos, const auto& e_size, const auto& e_rot) {
            m_renderer
                .drawQuad({e_pos.x, e_pos.y}, e_size, e_rot, enemy_texture, {1.f, 0.4f, 0.4f, 1.f});
        });

    const auto bullet_view = m_bulletRegistry.view<
        const ecs::component::position,
        const ecs::component::size,
        const ecs::component::rotation>(entt::exclude<ecs::component::destroyed>);
    bullet_view.each(
        [&theme_color,
         &m_renderer = m_renderer](const auto& b_pos, const auto& b_size, const auto& b_rot) {
            m_renderer.drawQuad({b_pos.x, b_pos.y}, b_size, b_rot, theme_color);
        });

    m_map.drawObjects({pos.x, pos.y});
    m_renderer.endBatch(m_camera.getProjectionMatrix(), m_camera.getViewMatrix());

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
            m_hero.maxHealth,
            current_ammo,
            total_ammo,
            m_hero.getCurrentItemIndex());
    }
    if(m_onLeaveStarted)
    {
        const auto TextCentered = [](const char* text, auto&&... args) {
            float font_size = ImGui::GetFontSize() * strlen(text) / 2;
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
            TextCentered("You won");
        }
        ImGui::End();
        font_guard.popFont();
    }

    if constexpr(not mono::config::constant::debugMode)
    {
        if(s_show_debug_info)
        {
            ImGui::Begin("Release Mode Statistics");
            {
                ImGui::Text(
                    "Performance: [%.2fms] [%.0ffps]",
                    1000.0f / ImGui::GetIO().Framerate,
                    ImGui::GetIO().Framerate);
                ImGui::Text(
                    "Mouse Position: Screen[%.2fx, %.2fy]",
                    ImGui::GetMousePos().x,
                    ImGui::GetMousePos().y);
                ImGui::Text("Quad count: %d", m_renderer.getStats().quadCount);
                ImGui::Text("Line count: %d", m_renderer.getStats().lineCount);
                ImGui::Text("Draw calls: %d", m_renderer.getStats().drawCount);
                ImGui::Text("Indices: %d", m_renderer.getStats().indexCount);
            }
            ImGui::End();
        }
    }
}

// returns world coordinates at 0 height
glm::vec2 GameplaySection::mouseScreenPosToWorldPos(const glm::vec2& mouse_pos, Camera& camera)
{
    const auto& inverse_projection_matrix = camera.getInverseProjectionMatrix();
    const auto& inverse_view_matrix = camera.getInverseViewMatrix();

    const auto& mouse_x = mouse_pos.x;
    const auto& mouse_y = mouse_pos.y;
    const auto& window_size = ResourceManager::window->getSize();
    const auto& window_w = window_size.x;
    const auto& window_h = window_size.y;

    const float norm_mouse_x = (2.f * mouse_x / window_w) - 1.f;
    const float norm_mouse_y = 1.f - (2.f * mouse_y / window_h);

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
bool GameplaySection::onEnter()
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
bool GameplaySection::onLeave()
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
