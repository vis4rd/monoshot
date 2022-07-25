#pragma once

#include "../utility/Printing.hpp"

namespace
{

using glm::vec3, glm::mat4;

class OrthogonalCamera final
{
    public:
    OrthogonalCamera() = default;
    explicit OrthogonalCamera(const vec3& position);
    OrthogonalCamera(const vec3& position, const float& zoom);
    // write copy, move ctors, write default dtor

    const float& getZoom() const;
    const vec3& getPosition() const;
    const mat4& getViewMatrix();
    void setZoom(const float& zoom);
    void setPosition(const vec3& position);

    private:
    vec3 m_position = vec3(0.f, 0.f, 10.f);  // position of the camera
    vec3 m_target = vec3(0.f, 0.f, 0.f);  // position where the camera points to
    vec3 m_up = vec3(0.0f, 1.0f, 0.0f);
    mat4 m_viewMatrix = glm::lookAt(m_position, m_target, m_up);
    // vec3 m_direction = normalize(m_position - m_target);  // direction which the camera is looking in
    float m_zoom = 1.f;
};

}  // namespace