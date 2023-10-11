#pragma once

// #include "../utility/VertexArray.hpp"
// #include "../texture/Texture2D.hpp"
#include <camera/PerspectiveCamera.hpp>
#include <entt/entity/registry.hpp>
#include <renderer/Renderer.hpp>
#include <section/Section.hpp>
#include <time/Timer.hpp>

#include "../ecs/systems.hpp"
#include "../gameplay/Hero.hpp"
#include "../map/Map.hpp"
#include "../ui/GameplayLayout.hpp"

class DebugSection final : public Section
{
    public:
    DebugSection();
    ~DebugSection();

    void update() noexcept override;
    void render() noexcept override;

    private:
    glm::vec2 mouseScreenPosToWorldPos(const glm::vec2& mouse_pos, Camera& camera);
    bool onEnter();
    bool onLeave();
    void showDebugUI();

    private:
    // VertexArray VAO;

    // content variables
    // glm::vec3 scale = {1.f, 1.f, 1.f};
    // float rotation = 0.f;
    // glm::vec3 position = {0.f, 0.f, 0.f};
    // glm::mat4 model_matrix = glm::identity<glm::mat4>();
    // std::shared_ptr<Texture2D> firstTexture;

    entt::registry m_mapElementsRegistry{};
    entt::registry m_bulletRegistry{};
    entt::registry m_enemyRegistry{};
    PerspectiveCamera m_camera;
    Renderer m_renderer;
    Map m_map;
    Hero m_hero;
    UI::GameplayLayout m_layout;

    bool m_onEnterFinished = false;
    bool m_onLeaveStarted = false;
    const double m_entranceDuration = 5.0;
    double m_enterFinishTimestamp = Timer::getTotalTime() + m_entranceDuration;
    const double m_leaveDuration = 5.0;
    double m_leaveStartTimestamp = 0.0;
};
