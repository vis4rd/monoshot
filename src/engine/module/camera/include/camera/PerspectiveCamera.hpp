#pragma once

#include "Camera.hpp"

class PerspectiveCamera : public Camera
{
    public:
    PerspectiveCamera() = default;
    explicit PerspectiveCamera(const glm::vec3& position, const glm::vec2& resolution);
    PerspectiveCamera(const PerspectiveCamera& copy);
    PerspectiveCamera(PerspectiveCamera&& move);
    ~PerspectiveCamera() override = default;

    PerspectiveCamera& operator=(const PerspectiveCamera& copy);
    PerspectiveCamera& operator=(PerspectiveCamera&& move);

    const float& getFov() const;
    float getZoom() const;
    void setFov(const float& fov);
    void setZoom(const float& zoom);

    protected:
    void updateViewMatrix() override;
    void updateProjectionMatrix() override;

    protected:
    float m_fov = 45.f;
};
