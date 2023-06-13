#pragma once

#include <camera/PerspectiveCamera.hpp>
#include <entt/entity/registry.hpp>
#include <section/Section.hpp>
#include <time/Timer.hpp>

#include "../gameplay/Hero.hpp"
#include "../map/Map.hpp"
#include "../ui/GameplayLayout.hpp"

class GameplaySection : public Section
{
    public:
    GameplaySection();
    virtual ~GameplaySection();

    void update() noexcept override;
    void render() noexcept override;

    protected:
    glm::vec2 mouseScreenPosToWorldPos(const glm::vec2& mouse_pos, Camera& camera);
    bool onEnter();
    bool onLeave();

    protected:
    PerspectiveCamera m_camera;
    Map m_map;
    Hero m_hero;
    UI::GameplayLayout m_layout;
    entt::registry m_mapElementsRegistry{};
    entt::registry m_bulletRegistry{};
    entt::registry m_enemyRegistry{};

    bool m_onEnterFinished = false;
    bool m_onLeaveStarted = false;
    const double m_entranceDuration = 5.0;
    double m_enterFinishTimestamp = Timer::getTotalTime() + m_entranceDuration;
    const double m_leaveDuration = 5.0;
    double m_leaveStartTimestamp = 0.0;
};
