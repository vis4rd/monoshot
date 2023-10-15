#pragma once

#include <glm/glm.hpp>
#include <resource/ResourceManager.hpp>
#include <traits/Scalar.hpp>

#include "../gameplay/items/Weapon.hpp"

namespace ecs::component
{

struct Size : public glm::vec2
{ };

struct Position : public glm::vec2
{ };

struct Rotation : public Scalar<float>
{ };

struct Velocity : public Scalar<float>
{ };

struct Direction : public glm::vec2
{ };  // direction of movement in degrees

struct Collision : public Scalar<bool>
{ };

struct MaxVelocity : public Scalar<float>
{
    explicit MaxVelocity(float val = 7.f)
        : Scalar<float>(val)
    { }
};

struct Acceleration : public Scalar<float>
{
    explicit Acceleration(float val = 100.f)
        : Scalar<float>(val)
    { }
};

struct Lifetime
{
    explicit Lifetime(double timestamp, double life_duration = 1.0)
        : creation(timestamp)
        , timeTillDeath(life_duration)
    { }

    double creation;
    double timeTillDeath;
};

struct Destroyed
{ };

struct Health : public Scalar<std::int32_t>
{
    explicit Health(std::int32_t val = 100)
        : Scalar<std::int32_t>(val)
    { }
};

struct AiState : public Scalar<std::int32_t>
{
    enum
    {
        IDLE = 0,
        AWARE = 1
    };

    AiState()
        : Scalar<std::int32_t>(IDLE)
    { }
};

struct AiAwareRange : public Scalar<float>
{
    explicit AiAwareRange(float val = 10.f)
        : Scalar<float>(val)
    { }
};

using AiWeapon = Weapon;

}  // namespace ecs::component
