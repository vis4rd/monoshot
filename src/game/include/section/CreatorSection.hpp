#pragma once

#include <camera/PerspectiveCamera.hpp>
#include <renderer/Renderer.hpp>
#include <section/Section.hpp>
#include <texture/Texture.hpp>

#include "../map/Map.hpp"

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
    entt::registry m_entities{};
    Renderer m_renderer;
    Map m_map;
    PerspectiveCamera m_camera;
};
