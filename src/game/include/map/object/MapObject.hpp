#pragma once

#include <memory>

#include <glm/glm.hpp>
#include <opengl/texture/Texture.hpp>

#include "../ObjectID.hpp"

class MapObject
{
    public:
    MapObject(
        const glm::vec2& position,
        const glm::vec2& size,
        std::shared_ptr<mono::Texture> texture,
        float rotation = 0.f,
        bool has_collision = true,
        float opacity_on_collision = 1.f);
    MapObject(const MapObject& copy) = default;
    MapObject(MapObject&& move) noexcept = default;
    ~MapObject() = default;

    MapObject& operator=(const MapObject& copy) = default;
    MapObject& operator=(MapObject&& move) noexcept = default;

    const glm::vec2& getPosition() const;
    const glm::vec2& getSize() const;
    const float getRotation() const;
    const std::shared_ptr<mono::Texture>& getTexture() const;
    void setPosition(const glm::vec2& new_pos);

    static MapObject createPredefined(ObjectID id, const glm::vec2& position, float rotation);
    static MapObject createSmallTree(const glm::vec2& position, float rotation);
    static MapObject createLargeTree(const glm::vec2& position, float rotation);

    public:
    bool hasCollision = true;
    ObjectID id;
    float opacityOnCollision = 1.f;

    private:
    glm::vec2 m_position{};
    glm::vec2 m_size{};
    float m_rotation = 0.f;
    std::shared_ptr<mono::Texture> m_texture;
};
