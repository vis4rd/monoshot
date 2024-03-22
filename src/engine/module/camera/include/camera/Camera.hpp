#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// model matrix: object space -> world space
// view matrix: world space -> camera space
// projection matrix: camera space -> clip space
// glViewport: clip space -> viewport space

class Camera
{
    public:
    Camera() = default;
    explicit Camera(const glm::vec3& position, const glm::vec2& resolution);
    Camera(const Camera& copy) = default;
    Camera(Camera&& move) noexcept;
    virtual ~Camera() = default;

    Camera& operator=(const Camera& copy) = default;
    Camera& operator=(Camera&& move) noexcept;

    virtual const glm::vec3& getPosition() const final;
    virtual const glm::vec3& getTargetPosition() const final;
    virtual const float& getNearPlane() const final;
    virtual const float& getFarPlane() const final;
    virtual const glm::mat4& getViewMatrix() final;
    virtual const glm::mat4& getProjectionMatrix() final;
    virtual const glm::mat4& getInverseViewMatrix() final;
    virtual const glm::mat4& getInverseProjectionMatrix() final;
    virtual void setPosition(const glm::vec3& position) final;
    virtual void setTarget(const glm::vec3& target_position) final;
    virtual void setNearPlane(const float& near_plane) final;
    virtual void setFarPlane(const float& far_plane) final;
    virtual void setResolution(const glm::vec2& resolution) final;

    protected:
    virtual void updateViewMatrix() = 0;
    virtual void updateProjectionMatrix() = 0;

    protected:
    glm::vec3 m_position{};  // position of the camera
    glm::vec3 m_target{};  // position where the camera points to
    glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 m_viewMatrix = glm::identity<glm::mat4>();
    glm::mat4 m_projectionMatrix = glm::identity<glm::mat4>();
    glm::mat4 m_inverseViewMatrix = glm::identity<glm::mat4>();
    glm::mat4 m_inverseProjectionMatrix = glm::identity<glm::mat4>();
    float m_nearPlane = 0.1f;
    float m_farPlane = 1000.f;
    glm::vec2 m_resolution{};
    bool m_viewMatrixNeedsUpdate = false;
    bool m_projectionMatrixNeedsUpdate = false;
};
