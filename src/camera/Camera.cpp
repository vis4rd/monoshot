#include "../../include/camera/Camera.hpp"

Camera::Camera(const glm::vec3& position, const glm::vec2& resolution)
    : m_position(position),
      m_target(m_position.x, m_position.y, 0.0f),
      m_resolution(resolution)
{
}

Camera::Camera(const Camera& copy)
    : m_position(copy.m_position),
      m_target(copy.m_target),
      m_up(copy.m_up),
      m_viewMatrix(copy.m_viewMatrix),
      m_projectionMatrix(copy.m_projectionMatrix),
      m_inverseViewMatrix(copy.m_inverseViewMatrix),
      m_inverseProjectionMatrix(copy.m_inverseProjectionMatrix),
      m_nearPlane(copy.m_nearPlane),
      m_farPlane(copy.m_farPlane),
      m_resolution(copy.m_resolution),
      m_viewMatrixNeedsUpdate(copy.m_viewMatrixNeedsUpdate),
      m_projectionMatrixNeedsUpdate(copy.m_projectionMatrixNeedsUpdate)
{
}

Camera::Camera(Camera&& move)
    : m_position(std::move(move.m_position)),
      m_target(std::move(move.m_target)),
      m_up(std::move(move.m_up)),
      m_viewMatrix(std::move(move.m_viewMatrix)),
      m_projectionMatrix(std::move(move.m_projectionMatrix)),
      m_inverseViewMatrix(std::move(move.m_inverseViewMatrix)),
      m_inverseProjectionMatrix(std::move(move.m_inverseProjectionMatrix)),
      m_nearPlane(std::move(move.m_nearPlane)),
      m_farPlane(std::move(move.m_farPlane)),
      m_resolution(std::move(move.m_resolution)),
      m_viewMatrixNeedsUpdate(std::move(move.m_viewMatrixNeedsUpdate)),
      m_projectionMatrixNeedsUpdate(std::move(move.m_projectionMatrixNeedsUpdate))
{
}

Camera& Camera::operator=(const Camera& copy)
{
    m_position = copy.m_position;
    m_target = copy.m_target;
    m_up = copy.m_up;
    m_viewMatrix = copy.m_viewMatrix;
    m_projectionMatrix = copy.m_projectionMatrix;
    m_inverseViewMatrix = copy.m_inverseViewMatrix;
    m_inverseProjectionMatrix = copy.m_inverseProjectionMatrix;
    m_nearPlane = copy.m_nearPlane;
    m_farPlane = copy.m_farPlane;
    m_resolution = copy.m_resolution;
    m_viewMatrixNeedsUpdate = copy.m_viewMatrixNeedsUpdate;
    m_projectionMatrixNeedsUpdate = copy.m_projectionMatrixNeedsUpdate;
    return *this;
}

Camera& Camera::operator=(Camera&& move)
{
    m_position = std::move(move.m_position);
    m_target = std::move(move.m_target);
    m_up = std::move(move.m_up);
    m_viewMatrix = std::move(move.m_viewMatrix);
    m_projectionMatrix = std::move(move.m_projectionMatrix);
    m_inverseViewMatrix = std::move(move.m_inverseViewMatrix);
    m_inverseProjectionMatrix = std::move(move.m_inverseProjectionMatrix);
    m_nearPlane = std::move(move.m_nearPlane);
    m_farPlane = std::move(move.m_farPlane);
    m_resolution = std::move(move.m_resolution);
    m_viewMatrixNeedsUpdate = std::move(move.m_viewMatrixNeedsUpdate);
    m_projectionMatrixNeedsUpdate = std::move(move.m_projectionMatrixNeedsUpdate);
    return *this;
}

const glm::vec3& Camera::getPosition() const
{
    return m_position;
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
