#pragma once

// #include "../utility/VertexArray.hpp"
// #include "../texture/Texture2D.hpp"
#include "../gameplay/Hero.hpp"
#include "../camera/PerspectiveCamera.hpp"
#include "../map/Map.hpp"
#include "Section.hpp"

#include <entt/entity/registry.hpp>

class DebugSection final : public Section
{
    public:
    DebugSection();
    ~DebugSection();

    void update() noexcept override;
    void render() noexcept override;

    private:
    glm::vec2 mouseScreenPosToWorldPos(const glm::vec2& mouse_pos, Camera& camera);

    private:
    // VertexArray VAO;

    // content variables
    // glm::vec3 scale = {1.f, 1.f, 1.f};
    // float rotation = 0.f;
    // glm::vec3 position = {0.f, 0.f, 0.f};
    // glm::mat4 model_matrix = glm::identity<glm::mat4>();
    // std::shared_ptr<Texture2D> firstTexture;

    PerspectiveCamera m_camera;
    Map m_mapGrid;
    Hero m_hero;

    entt::registry m_registry;
    entt::entity m_heroEntity;
};
