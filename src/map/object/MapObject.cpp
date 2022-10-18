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

MapObject MapObject::createPredefined(const glm::vec2& position, ObjectID id, bool randomize)
{
    using res = ResourceManager;

    static float random_rotation;
    if(randomize)
    {
        random_rotation = random::getRandomNumber(0.f, 360.f);
    }
    std::shared_ptr<Texture2D> texture;
    bool has_collision = false;
    glm::vec2 size{};
    switch(id)
    {
        case ObjectID::Car:
        {
            texture = res::carTexture;
            size = {0.f, 0.f};
            has_collision = false;
            break;
        }
        case ObjectID::DestroyedCar:
        {
            texture = res::destroyedCarTexture;
            size = {0.f, 0.f};
            has_collision = false;
            break;
        }
        case ObjectID::Chair:
        {
            texture = res::chairTexture;
            size = {0.f, 0.f};
            has_collision = false;
            break;
        }
        case ObjectID::OutdoorBench:
        {
            texture = res::outdoorBenchTexture;
            size = {0.f, 0.f};
            has_collision = false;
            break;
        }
        case ObjectID::Table:
        {
            texture = res::tableTexture;
            size = {0.f, 0.f};
            has_collision = false;
            break;
        }
        case ObjectID::SmallTree:
        {
            texture = res::smallTreeTexture;
            size = {0.f, 0.f};
            has_collision = false;
            break;
        }
        case ObjectID::LargeTree:
        {
            texture = res::largeTreeTexture;
            size = {10.f, 10.f};
            has_collision = false;
            break;
        }
        case ObjectID::SmallBush:
        {
            texture = res::smallBushTexture;
            size = {0.f, 0.f};
            has_collision = false;
            break;
        }
        case ObjectID::LargeBush:
        {
            texture = res::largeBushTexture;
            size = {0.f, 0.f};
            has_collision = false;
            break;
        }
        default:
        {
            size = {0.f, 0.f};
            has_collision = false;
            break;
        }
    }
    auto retval = MapObject(position, size, std::move(texture), random_rotation, has_collision);
    retval.id = id;
    return retval;
}

MapObject MapObject::createLargeTree(const glm::vec2& position)
{
    return MapObject::createPredefined(position, ObjectID::LargeTree);
}
