#pragma once

#include <glm/glm.hpp>

namespace ecs::component
{
// clang-format off

struct position
{
    glm::vec2 m_position = {};
    position(glm::vec2 position = {}) : m_position(position) { }
    operator glm::vec2&() { return m_position; }
};

struct rotation
{
    float m_rotation = 0.f;
    rotation(float rotation = 0.f) : m_rotation(rotation) { }
    operator float&() { return m_rotation; }
};

struct velocity
{
    float m_velocity = 0.f;
    float m_maxVelocity = 100.f;
    velocity(float velocity = 0.f) : m_velocity(velocity) { }
    operator float&() { return m_velocity; }
};

struct acceleration
{
    float m_acceleration = 1.f;
    acceleration(float acceleration = 1.f) : m_acceleration(acceleration) { }
    operator float&() { return m_acceleration; }
};

// clang-format on
}  // namespace ecs::component
