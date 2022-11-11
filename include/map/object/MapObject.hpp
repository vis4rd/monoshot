#pragma once

#include "../../texture/Texture.hpp"
#include "../ObjectID.hpp"

#include <glm/glm.hpp>
#include <memory>

class MapObject
{
    public:
    MapObject(const glm::vec2& position, const glm::vec2& size, Texture::Texture texture, float rotation = 0.f, bool has_collision = true, float opacity_on_collision = 1.f);
    ~MapObject() = default;

    const glm::vec2& getPosition() const;
    const glm::vec2& getSize() const;
    const float getRotation() const;
    const Texture::Texture& getTexture() const;
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
    Texture::Texture m_texture;
};
