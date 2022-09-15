#include "../../include/ecs/systems.hpp"

namespace ecs::system
{

void move(entt::registry &registry, const float &delta_time)
{
    using namespace ecs::component;
    auto view = registry.view<position, rotation, velocity, acceleration>();
    for(const auto &entity : view)
    {
        auto &pos = view.get<position>(entity);
        auto &rot = view.get<rotation>(entity);
        auto &vel = view.get<velocity>(entity);
        auto &acc = view.get<acceleration>(entity);

        vel.m_velocity = glm::min(vel + acc * delta_time, vel.m_maxVelocity * delta_time);
        auto rad = glm::radians(rot.m_rotation);
        glm::vec2 rot_vec = {glm::cos(rad), glm::sin(rad)};
        pos.m_position += (vel.m_velocity * rot_vec * delta_time);
    }
}

}  // namespace ecs::system
