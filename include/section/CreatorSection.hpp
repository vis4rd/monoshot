#pragma once

#include "../utility/VertexArray.hpp"
#include "../map/Map.hpp"
#include "../camera/PerspectiveCamera.hpp"
#include "Section.hpp"

class CreatorSection final : public Section
{
    public:
    CreatorSection();
    ~CreatorSection();

    void update() noexcept override;
    void render() noexcept override;

    private:
    glm::vec2 mouseScreenPosToWorldPos(const glm::vec2& mouse_pos, Camera& camera);

    private:
    Map m_map;
    PerspectiveCamera m_camera;
};
