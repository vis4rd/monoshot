#include "../../include/camera/OrthogonalCamera.hpp"

namespace
{

OrthogonalCamera::OrthogonalCamera(const vec3& position)
    : m_position(position),
      m_target(m_position.x, m_position.y, 0.0f)
{
}

OrthogonalCamera::OrthogonalCamera(const vec3& position, const float& zoom)
    : m_position(position),
      m_target(m_position.x, m_position.y, 0.0f),
      m_zoom(zoom)
{
}

const float& OrthogonalCamera::getZoom() const
{
    return m_zoom;
}

const vec3& OrthogonalCamera::getPosition() const
{
    return m_position;
}

const mat4& OrthogonalCamera::getViewMatrix()
{
    m_viewMatrix = glm::lookAt(m_position, m_target, m_up);
    return m_viewMatrix;
}

void OrthogonalCamera::setZoom(const float& zoom)
{
    m_zoom = zoom;
    if(m_zoom < 0.f)
    {
        m_zoom = 0.1f;
    }
    if(m_zoom > 1000.f)
    {
        m_zoom = 1000.f;
    }
}

void OrthogonalCamera::setPosition(const vec3& position)
{
    m_position = position;
}

}  // namespace