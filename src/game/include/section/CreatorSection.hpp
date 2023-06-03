#pragma once

#include "../../module/renderer/opengl/include/texture/Texture.hpp"
#include "../camera/PerspectiveCamera.hpp"
#include "../map/Map.hpp"
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
    entt::registry m_entities{};
};
