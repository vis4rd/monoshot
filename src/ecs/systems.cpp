#include "../../include/ecs/systems.hpp"

namespace ecs::system
{

void move(entt::registry &registry, const float &delta_time)
{
    using namespace ecs::component;
    auto view = registry.view<position, rotation, velocity, max_velocity, acceleration>();
    for(const auto &entity : view)
    {
        auto &pos = view.get<position>(entity);
        auto &rot = view.get<rotation>(entity);
        auto &vel = view.get<velocity>(entity);
        auto &mvel = view.get<max_velocity>(entity);
        auto &acc = view.get<acceleration>(entity);

        vel.data = glm::min(vel + acc * delta_time, mvel * delta_time);
        auto rad = glm::radians(rot.data);
        glm::vec2 rot_vec = {glm::cos(rad), glm::sin(rad)};
        pos.x += (vel * rot_vec.x * delta_time);
        pos.y += (vel * rot_vec.y * delta_time);
    }
}

}  // namespace ecs::system
