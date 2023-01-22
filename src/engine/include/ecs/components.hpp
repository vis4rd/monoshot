#pragma once

#include <glm/glm.hpp>

#include "../utility/ResourceManager.hpp"
#include "../traits/Scalar.hpp"
#include "../gameplay/items/Weapon.hpp"

namespace ecs::component
{

struct size : public glm::vec2
{
};

struct position : public glm::vec2
{
};

struct rotation : public Scalar<float>
{
};

struct velocity : public Scalar<float>
{
};

struct direction : public glm::vec2
{
};  // direction of movement in degrees

struct collision : public Scalar<bool>
{
};

struct max_velocity : public Scalar<float>
{
    max_velocity(float val = 7.f)
        : Scalar<float>(val)
    {
    }
};

struct acceleration : public Scalar<float>
{
    acceleration(float val = 100.f)
        : Scalar<float>(val)
    {
    }
};

struct lifetime
{
    lifetime(double timestamp, double life_duration = 1.0)
        : creation(timestamp),
          timeTillDeath(life_duration)
    {
    }

    double creation;
    double timeTillDeath;
};

struct destroyed
{
};

struct health : public Scalar<std::int32_t>
{
    health(std::int32_t val = 100)
        : Scalar<std::int32_t>(val)
    {
    }
};

struct ai_state : public Scalar<std::int32_t>
{
    enum
    {
        IDLE = 0,
        AWARE = 1
    };

    ai_state()
        : Scalar<std::int32_t>(IDLE)
    {
    }
};

struct ai_aware_range : public Scalar<float>
{
    ai_aware_range(float val = 10.f)
        : Scalar<float>(val)
    {
    }
};

using ai_weapon = Weapon;

}  // namespace ecs::component
