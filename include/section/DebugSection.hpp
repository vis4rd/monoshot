#pragma once

#include "../ui/elements/LowerNavigationBox.hpp"
#include "../utility/VertexArray.hpp"
#include "../texture/Texture2D.hpp"

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
    VertexArray VAO;

    // content variables
    glm::vec3 scale = {1.f, 1.f, 1.f};
    float rotation = 0.f;
    glm::vec3 position = {0.f, 0.f, 0.f};
    glm::mat4 model_matrix = glm::identity<glm::mat4>();
    PerspectiveCamera m_camera;

    float vertices[12] = {-0.5f, -0.5f, 0.f, 0.5f, -0.5f, 0.f, 0.5f, 0.5f, 0.f, -0.5f, 0.5f, 0.f};
    uint32_t indices[6] = {0, 1, 2, 2, 3, 0};
    Map m_mapGrid;

    Texture2D firstTexture;
};
