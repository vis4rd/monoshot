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
    CreatorSection(const CreatorSection&) = delete;
    CreatorSection(CreatorSection&&) = delete;
    ~CreatorSection() override;

    CreatorSection& operator=(const CreatorSection&) = delete;
    CreatorSection& operator=(CreatorSection&&) = delete;

    void update() noexcept override;
    void render() noexcept override;

    private:
    glm::vec2 mouseScreenPosToWorldPos(const glm::vec2& mouse_pos, Camera& camera);

    private:
    entt::registry m_entities{};
    Renderer m_renderer;
    Map m_map;
    PerspectiveCamera m_camera;
    std::size_t m_selectedMapItem{BlockID::Wall};
    bool m_selectedSolid{false};
    glm::vec2 m_mouseWorldPos{0.f, 0.f};
    float m_randomizedRotation{};
    glm::vec2 m_endAreaSize{10.f, 10.f};
};
