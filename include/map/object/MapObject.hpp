#pragma once

#include "../../texture/Texture2D.hpp"
#include "../ObjectID.hpp"

#include <glm/glm.hpp>
#include <memory>

class MapObject
{
    public:
    MapObject(const glm::vec2& position, const glm::vec2& size, std::shared_ptr<Texture2D> texture, float rotation = 0.f, bool has_collision = true, float opacity_on_collision = 1.f);
    ~MapObject() = default;

    const glm::vec2& getPosition() const;
    const glm::vec2& getSize() const;
    const float getRotation() const;
    const std::shared_ptr<Texture2D>& getTexture() const;
    void setPosition(const glm::vec2& new_pos);

    static MapObject createPredefined(const glm::vec2& position, ObjectID id, bool randomize = false);
    static MapObject createLargeTree(const glm::vec2& position);

    public:
    bool hasCollision = true;
    ObjectID id;
    float opacityOnCollision = 1.f;

    private:
    glm::vec2 m_position{};
    glm::vec2 m_size{};
    float m_rotation = 0.f;
    std::shared_ptr<Texture2D> m_texture;
};
