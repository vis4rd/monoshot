#pragma once

#include <camera/PerspectiveCamera.hpp>
#include <entt/entity/registry.hpp>
#include <section/Section.hpp>
#include <time/Timer.hpp>

#include "../gameplay/Hero.hpp"
#include "../map/Map.hpp"
#include "../ui/GameplayLayout.hpp"

namespace mono
{
class Renderer;
}

class DebugSection final : public Section
{
    public:
    DebugSection();
    DebugSection(const DebugSection&) = delete;
    DebugSection(DebugSection&&) = delete;
    ~DebugSection() override;

    DebugSection& operator=(const DebugSection&) = delete;
    DebugSection& operator=(DebugSection&&) = delete;

    void update() noexcept override;
    void render() noexcept override;

    private:
    glm::vec2 mouseScreenPosToWorldPos(const glm::vec2& mouse_pos, Camera& camera);
    bool onEnter();
    bool onLeave();
    void showDebugUI(bool& draw_area, bool& draw_bounding_boxes);

    private:
    entt::registry m_mapElementsRegistry{};
    entt::registry m_bulletRegistry{};
    entt::registry m_enemyRegistry{};
    PerspectiveCamera m_camera;
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
