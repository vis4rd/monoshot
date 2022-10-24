#include "../../../include/map/object/MapObject.hpp"

#include "../../../include/utility/ResourceManager.hpp"

MapObject::MapObject(const glm::vec2& position, const glm::vec2& size, std::shared_ptr<Texture2D> texture, float rotation, bool has_collision, float opacity_on_collision)
    : m_position(position),
      m_size(size),
      m_rotation(rotation),
      m_texture(texture),
      hasCollision(has_collision),
      opacityOnCollision(opacity_on_collision)
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

MapObject MapObject::createPredefined(ObjectID id, const glm::vec2& position, float rotation)
{
    using res = ResourceManager;

    std::shared_ptr<Texture2D> texture;
    bool has_collision = false;
    glm::vec2 size{};
    float opacity_on_collision = 1.f;  // domain <0; 1>
    switch(id)
    {
        case ObjectID::Car:
        {
            texture = res::carTexture;
            size = {2.f, 4.f};
            has_collision = true;
            opacity_on_collision = 1.f;
            break;
        }
        case ObjectID::DestroyedCar:
        {
            texture = res::destroyedCarTexture;
            size = {2.f, 4.f};
            has_collision = true;
            opacity_on_collision = 1.f;
            break;
        }
        case ObjectID::Chair:
        {
            texture = res::chairTexture;
            size = {1.f, 1.f};
            has_collision = true;
            opacity_on_collision = 1.f;
            break;
        }
        case ObjectID::OutdoorBench:
        {
            texture = res::outdoorBenchTexture;
            size = {3.f, 1.f};
            has_collision = true;
            opacity_on_collision = 1.f;
            break;
        }
        case ObjectID::Table:
        {
            texture = res::tableTexture;
            size = {2.f, 2.f};
            has_collision = true;
            opacity_on_collision = 1.f;
            break;
        }
        case ObjectID::SmallTree:
        {
            texture = res::smallTreeTexture;
            size = {5.f, 5.f};
            has_collision = false;
            opacity_on_collision = 0.7f;
            break;
        }
        case ObjectID::LargeTree:
        {
            texture = res::largeTreeTexture;
            size = {10.f, 10.f};
            has_collision = false;
            opacity_on_collision = 0.7f;
            break;
        }
        case ObjectID::SmallBush:
        {
            texture = res::smallBushTexture;
            size = {3.f, 3.f};
            has_collision = false;
            opacity_on_collision = 0.7f;
            break;
        }
        case ObjectID::LargeBush:
        {
            texture = res::largeBushTexture;
            size = {4.f, 4.f};
            has_collision = false;
            opacity_on_collision = 0.7f;
            break;
        }
        default:
        {
            size = {0.f, 0.f};
            has_collision = false;
            opacity_on_collision = 1.f;
            break;
        }
    }
    auto retval = MapObject(position, size, std::move(texture), rotation, has_collision, opacity_on_collision);
    retval.id = id;
    return std::move(retval);
}

MapObject MapObject::createSmallTree(const glm::vec2& position, float rotation)
{
    return MapObject::createPredefined(ObjectID::SmallTree, position, rotation);
}

MapObject MapObject::createLargeTree(const glm::vec2& position, float rotation)
{
    return MapObject::createPredefined(ObjectID::LargeTree, position, rotation);
}
