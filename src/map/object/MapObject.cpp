#include "../../../include/map/object/MapObject.hpp"

#include "../../../include/utility/ResourceManager.hpp"
#include "../../../include/utility/RandomNumber.hpp"

MapObject::MapObject(const glm::vec2& position, const glm::vec2& size, std::shared_ptr<Texture2D> texture, float rotation, bool has_collision)
    : m_position(position),
      m_size(size),
      m_rotation(rotation),
      m_texture(texture),
      hasCollision(has_collision)
{
    id = ObjectID::FIRST_OBJECT;
}

const glm::vec2& MapObject::getPosition() const
{
    return m_position;
}

const glm::vec2& MapObject::getSize() const
{
    return m_size;
}

const float MapObject::getRotation() const
{
    return m_rotation;
}

const std::shared_ptr<Texture2D>& MapObject::getTexture() const
{
    return m_texture;
}

void MapObject::setPosition(const glm::vec2& new_pos)
{
    m_position = new_pos;
}

MapObject MapObject::createLargeTree(const glm::vec2& position)
{
    const auto random_rotation = random::getRandomNumber(0.f, 360.f);
    auto retval = MapObject(position, {10.f, 10.f}, ResourceManager::largeTreeTexture, random_rotation, false);
    retval.id = ObjectID::LargeTree;
    return retval;
}
