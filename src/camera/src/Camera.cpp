#include "../include/camera/Camera.hpp"

Camera::Camera(const glm::vec3& position, const glm::vec2& resolution)
    : m_position(position)
    , m_target(m_position.x, m_position.y, 0.0f)
    , m_resolution(resolution)
{ }

Camera::Camera(Camera&& move) noexcept
    : m_position(move.m_position)
    , m_target(move.m_target)
    , m_up(move.m_up)
    , m_viewMatrix(move.m_viewMatrix)
    , m_projectionMatrix(move.m_projectionMatrix)
    , m_inverseViewMatrix(move.m_inverseViewMatrix)
    , m_inverseProjectionMatrix(move.m_inverseProjectionMatrix)
    , m_nearPlane(move.m_nearPlane)
    , m_farPlane(move.m_farPlane)
    , m_resolution(move.m_resolution)
    , m_viewMatrixNeedsUpdate(move.m_viewMatrixNeedsUpdate)
    , m_projectionMatrixNeedsUpdate(move.m_projectionMatrixNeedsUpdate)
{ }

Camera& Camera::operator=(Camera&& move) noexcept
{
    m_position = move.m_position;
    m_target = move.m_target;
    m_up = move.m_up;
    m_viewMatrix = move.m_viewMatrix;
    m_projectionMatrix = move.m_projectionMatrix;
    m_inverseViewMatrix = move.m_inverseViewMatrix;
    m_inverseProjectionMatrix = move.m_inverseProjectionMatrix;
    m_nearPlane = move.m_nearPlane;
    m_farPlane = move.m_farPlane;
    m_resolution = move.m_resolution;
    m_viewMatrixNeedsUpdate = move.m_viewMatrixNeedsUpdate;
    m_projectionMatrixNeedsUpdate = move.m_projectionMatrixNeedsUpdate;
    return *this;
}

const glm::vec3& Camera::getPosition() const
{
    return m_position;
}

const glm::vec3& Camera::getTargetPosition() const
{
    return m_target;
}

const float& Camera::getNearPlane() const
{
    return m_nearPlane;
}

const float& Camera::getFarPlane() const
{
    return m_farPlane;
}

const glm::mat4& Camera::getViewMatrix()
{
    if(m_viewMatrixNeedsUpdate)
    {
        this->updateViewMatrix();
        m_viewMatrixNeedsUpdate = false;
    }
    return m_viewMatrix;
}

const glm::mat4& Camera::getProjectionMatrix()
{
    if(m_projectionMatrixNeedsUpdate)
    {
        this->updateProjectionMatrix();
        m_projectionMatrixNeedsUpdate = false;
    }
    return m_projectionMatrix;
}

const glm::mat4& Camera::getInverseViewMatrix()
{
    if(m_viewMatrixNeedsUpdate)
    {
        this->updateViewMatrix();
        m_viewMatrixNeedsUpdate = false;
    }
    return m_inverseViewMatrix;
}

const glm::mat4& Camera::getInverseProjectionMatrix()
{
    if(m_projectionMatrixNeedsUpdate)
    {
        m_inverseProjectionMatrix = glm::inverse(this->getProjectionMatrix());
    }
    return m_inverseProjectionMatrix;
}

void Camera::setPosition(const glm::vec3& position)
{
    m_position = position;
    m_viewMatrixNeedsUpdate = true;
}

void Camera::setTarget(const glm::vec3& target_position)
{
    m_target = target_position;
    m_viewMatrixNeedsUpdate = true;
}

void Camera::setNearPlane(const float& near_plane)
{
    m_nearPlane = near_plane;
    m_projectionMatrixNeedsUpdate = true;
}

void Camera::setFarPlane(const float& far_plane)
{
    m_farPlane = far_plane;
    m_projectionMatrixNeedsUpdate = true;
}

void Camera::setResolution(const glm::vec2& resolution)
{
    m_resolution = resolution;
    m_projectionMatrixNeedsUpdate = true;
}

void Camera::updateViewMatrix()
{
    m_viewMatrix = glm::lookAt(m_position, m_target, m_up);
    m_inverseViewMatrix = glm::inverse(m_viewMatrix);
}
