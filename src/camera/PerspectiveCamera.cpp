#include "../../include/camera/PerspectiveCamera.hpp"

PerspectiveCamera::PerspectiveCamera(const glm::vec3& position, const glm::vec2& resolution)
    : Camera(position, resolution)
{
    this->updateViewMatrix();
    this->updateProjectionMatrix();
}

PerspectiveCamera::PerspectiveCamera(const PerspectiveCamera& copy)
    : Camera(copy),
      m_fov(copy.m_fov)
{
}

PerspectiveCamera::PerspectiveCamera(PerspectiveCamera&& move)
    : Camera(std::move(move)),
      m_fov(std::move(move.m_fov))
{
}

PerspectiveCamera& PerspectiveCamera::operator=(const PerspectiveCamera& copy)
{
    this->Camera::operator=(copy);
    m_fov = copy.m_fov;
    return *this;
}

PerspectiveCamera& PerspectiveCamera::operator=(PerspectiveCamera&& move)
{
    this->Camera::operator=(std::move(move));
    m_fov = std::move(move.m_fov);
    return *this;
}

const float& PerspectiveCamera::getFov() const
{
    return m_fov;
}

float PerspectiveCamera::getZoom() const
{
    return 45.f / m_fov;
}

void PerspectiveCamera::setFov(const float& fov)
{
    m_fov = fov;
    if(m_fov < 1.f)
    {
        m_fov = 1.f;
    }
    else if(m_fov > 180.f)
    {
        m_fov = 180.f;
    }
    m_projectionMatrixNeedsUpdate = true;
}

void PerspectiveCamera::setZoom(const float& zoom)
{
    this->setFov(45.f / zoom);
    m_projectionMatrixNeedsUpdate = true;
}

void PerspectiveCamera::updateViewMatrix()
{
    Camera::updateViewMatrix();
}

void PerspectiveCamera::updateProjectionMatrix()
{
    m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_resolution.x / m_resolution.y, m_nearPlane, m_farPlane);
}
