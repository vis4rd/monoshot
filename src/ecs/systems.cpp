#include "../../include/ecs/systems.hpp"

namespace ecs::system
{

void move(entt::registry &registry, const float &delta_time)
{
    using namespace ecs::component;
    auto view = registry.view<position, rotation, velocity, max_velocity, acceleration>();
    for(const auto &entity : view)
    {
        glm::vec2 &pos = view.get<position>(entity);
        float &rot = view.get<rotation>(entity);
        float &vel = view.get<velocity>(entity);
        float &mvel = view.get<max_velocity>(entity);
        float &acc = view.get<acceleration>(entity);

        vel = glm::min(vel + acc * delta_time, mvel * delta_time);
        auto rad = glm::radians(rot);
        glm::vec2 rot_vec = {glm::cos(rad), glm::sin(rad)};
        pos += (vel * rot_vec * delta_time);
    }
}

}  // namespace ecs::system
