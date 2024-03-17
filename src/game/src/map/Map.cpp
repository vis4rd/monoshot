#include "../../include/map/Map.hpp"

#include <fstream>
#include <numeric>

#include <renderer/Renderer.hpp>
#include <resource/ResourceManager.hpp>

#include "../../include/ecs/actions.hpp"
#include "../../include/ecs/systems.hpp"

Map::Map(const std::size_t& width, const std::size_t& height)
    : m_width(width)
    , m_height(height)
    , m_tiles()
    , m_objects()
    , m_endArea(nullptr)
{
    m_tiles.reserve(width * height);

    this->setTheme(MapThemes::forestTheme);
}

Map::~Map()
{
    spdlog::trace("Deleting Map instance");
}

std::size_t Map::getSize() const
{
    return m_width * m_height;
}

const std::size_t& Map::getWidth() const
{
    return m_width;
}

const std::size_t& Map::getHeight() const
{
    return m_height;
}

void Map::addObject(const glm::vec2& position, const float& rotation, ObjectID object_id)
{
    spdlog::debug(
        "Placed MapObject: OID = {}, pos = ({}, {}), rot = {}",
        objectIdToString(object_id),
        position.x,
        position.y,
        rotation);
    m_objects.push_back(std::move(MapObject::createPredefined(object_id, position, rotation)));
}

void Map::removeObject(const glm::vec2& position)
{
    std::size_t dynamic_size = m_objects.size();
    for(std::int64_t i = 0; i < dynamic_size; i++)
    {
        const bool col = AABB::isColliding(
            position,
            {0.01f, 0.01f},
            m_objects[i].getPosition(),
            m_objects[i].getSize());
        if(col)
        {
            m_objects.erase(m_objects.begin() + i);
            i--;
            dynamic_size = m_objects.size();
        }
    }
}

void Map::setTile(const Tile& tile)
{
    spdlog::debug(
        "Map: Placing a tile with coords ({}, {}), rotation {}, block_id {}",
        tile.x,
        tile.y,
        tile.rotation,
        static_cast<std::size_t>(tile.blockId));
    this->calculateNewSize(tile.x, tile.y);

    const auto iter = this->findTile(tile.x, tile.y);
    if(iter != m_tiles.end())
    {
        *iter = tile;
    }
    else
    {
        m_tiles.push_back(tile);
    }
}

void Map::setTile(
    const float& x,
    const float& y,
    const float& rotation,
    BlockID block_id,
    const bool& solid)
{
    Tile new_tile;
    new_tile.x = std::round(x);
    new_tile.y = std::round(y);
    new_tile.rotation = rotation;
    new_tile.blockId = block_id;
    new_tile.solid = solid;
    this->setTile(new_tile);
}

void Map::removeTile(const float& x, const float& y)
{
    const auto iter = this->findTile(x, y);
    if(iter != m_tiles.end())
    {
        m_tiles.erase(iter);
    }
}

std::vector<Tile>::iterator Map::findTile(const float& x, const float& y)
{
    return std::find_if(m_tiles.begin(), m_tiles.end(), [&x, &y](const Tile tile1) {
        return (std::abs(tile1.x - std::round(x)) < 0.1f)
               && (std::abs(tile1.y - std::round(y)) < 0.1f);
    });
}

void Map::addTilesToRegistry(entt::registry& registry) const
{
    for(const auto& tile : m_tiles)
    {
        if(tile.solid == false)
        {
            continue;
        }
        auto entity = registry.create();
        auto& pos = registry.emplace<ecs::component::Position>(entity);
        auto& rot = registry.emplace<ecs::component::Rotation>(entity);
        glm::vec2& size = registry.emplace<ecs::component::Size>(entity);

        pos.x = tile.x;
        pos.y = tile.y;
        rot.data = tile.rotation;
        size = {1.f, 1.f};
        spdlog::debug(
            "Filling ECS registry with tile pos = ({}, {}), rot = {}",
            pos.x,
            pos.y,
            rot.data);
    }
    for(const auto& object : m_objects)
    {
        if(!object.hasCollision)
        {
            continue;
        }
        auto entity = registry.create();
        glm::vec2& pos = registry.emplace<ecs::component::Position>(entity);
        float& rot = registry.emplace<ecs::component::Rotation>(entity);
        glm::vec2& size = registry.emplace<ecs::component::Size>(entity);

        pos = object.getPosition();
        rot = object.getRotation();
        size = object.getSize();
        spdlog::debug("Filling ECS registry with tile pos = ({}, {}), rot = {}", pos.x, pos.y, rot);
    }
}

void Map::loadFromFile(const std::string& filename, entt::registry& enemy_registry)
{
    spdlog::debug("Loading map from file '{}'...", filename);

    std::ifstream file(filename);
    if(!file.is_open() || !file.good())
    {
        spdlog::debug("Couldn't open file: {}", filename);
        return;
    }

    std::stringstream file_buffer;
    file_buffer << file.rdbuf();
    file.close();

    float tile_x = 0;
    float tile_y = 0;
    float tile_rotation = 0.f;
    std::size_t tile_block = 0;
    bool tile_solid = false;

    std::string line{};
    while(std::getline(file_buffer, line))
    {
        if(line.empty())
        {
            break;
        }
        std::stringstream line_buffer(line);
        line_buffer >> tile_x >> tile_y >> tile_rotation >> tile_block >> tile_solid;
        this->setTile(tile_x, tile_y, tile_rotation, static_cast<BlockID>(tile_block), tile_solid);
        spdlog::debug(
            "Loading Tile: pos = ({}, {}), rot = {}, solid = {}, ID = '{}'",
            tile_x,
            tile_y,
            tile_rotation,
            tile_solid,
            blockToString(tile_block));
    }

    float object_pos_x = 0.f;
    float object_pos_y = 0.f;
    float object_size_x = 0.f;
    float object_size_y = 0.f;
    float object_rotation = 0.f;
    bool object_solid = false;
    std::size_t object_id = 2000;
    while(std::getline(file_buffer, line))
    {
        if(line.empty())
        {
            break;
        }
        std::stringstream line_buffer(line);
        line_buffer >> object_pos_x >> object_pos_y >> object_size_x >> object_size_y
            >> object_rotation >> object_solid >> object_id;
        if(object_id == 9999)
        {
            m_endArea = std::make_unique<OBB::Polygon>(
                glm::vec2(object_pos_x, object_pos_y),
                glm::vec2(object_size_x, object_size_y),
                object_rotation);
            continue;
        }
        this->addObject(
            {object_pos_x, object_pos_y},
            object_rotation,
            static_cast<ObjectID>(object_id));
        spdlog::debug(
            "Loading MapObject: pos = ({}, {}), size = ({}, {}), rot = {}, solid = {}, ID = '{}'",
            object_pos_x,
            object_pos_y,
            object_size_x,
            object_size_y,
            object_rotation,
            object_solid,
            objectIdToString(object_id));
    }

    float enemy_pos_x = 0.f;
    float enemy_pos_y = 0.f;
    float enemy_rotation = 0.f;
    while(std::getline(file_buffer, line))
    {
        std::stringstream line_buffer(line);
        line_buffer >> enemy_pos_x >> enemy_pos_y >> enemy_rotation;
        ecs::action::spawnEnemy(
            enemy_registry,
            {enemy_pos_x, enemy_pos_y},
            {1.f, 1.f},
            enemy_rotation);
        spdlog::debug("Loading Enemy: pos = ({}, {})", enemy_pos_x, enemy_pos_y);
    }

    spdlog::debug("Map loaded from file successfully");
}

void Map::saveToFile(const std::string& filename, const entt::registry& enemy_registry)
{
    spdlog::debug("Saving map to file '{}'...", filename);

    // clang-format off
    std::sort(m_tiles.begin(), m_tiles.end(), [](const Tile& tile1, const Tile& tile2)
    {
        auto tie = [](const Tile& tile) { return std::tie(tile.x, tile.y); };
        return tie(tile1) < tie(tile2);
    });
    // clang-format on

    // save tiles
    std::stringstream file_buffer{};
    for(const auto& tile : m_tiles)
    {
        file_buffer << tile.x << ' ' << tile.y << ' ' << tile.rotation << ' ' << tile.blockId << ' '
                    << tile.solid << '\n';
        spdlog::debug(
            "Saving Tile: pos = ({}, {}), rot = {}, solid = {}, ID = '{}'",
            tile.x,
            tile.y,
            tile.rotation,
            tile.solid,
            blockToString(tile.blockId));
    }

    file_buffer << "\n";  // create an empty line between tiles and objects in a file

    // save objects
    for(const auto& object : m_objects)
    {
        const auto& pos = object.getPosition();
        const auto& size = object.getSize();
        file_buffer << pos.x << ' ' << pos.y << ' ' << size.x << ' ' << size.y << ' '
                    << object.getRotation() << ' ' << object.hasCollision << ' ' << object.id
                    << '\n';
        spdlog::debug(
            "Saving MapObject: pos = ({}, {}), size = ({}, {}), rot = {}, solid = {}, ID = '{}'",
            pos.x,
            pos.y,
            size.x,
            size.y,
            object.getRotation(),
            object.hasCollision,
            objectIdToString(object.id));
    }

    if(m_endArea)
    {
        file_buffer << m_endArea->position.x << ' ' << m_endArea->position.y << ' '
                    << m_endArea->size.x << ' ' << m_endArea->size.y << ' ' << 0.f << ' ' << 0
                    << ' ' << 9999 << '\n';
        spdlog::debug(
            "Saving EndArea: pos = ({}, {}), size = ({}, {})",
            m_endArea->position.x,
            m_endArea->position.y,
            m_endArea->size.x,
            m_endArea->size.y);
    }

    file_buffer << "\n";  // create an empty line between objects and enemies in a file

    auto view =
        enemy_registry.view<const ecs::component::Position, const ecs::component::Rotation>();
    for(auto&& [enemy, pos, rot] : view.each())
    {
        file_buffer << pos.x << ' ' << pos.y << ' ' << rot.data << '\n';
        spdlog::debug("Saving Enemy: pos = ({}, {}), rot = {}", pos.x, pos.y, rot.data);
    }

    // dump buffer to the file
    std::ofstream file(filename);
    if(!file.is_open() || !file.good())
    {
        spdlog::error("Could not open file '{}', can not save the map", filename);
        return;
    }
    file << file_buffer.rdbuf();
    file.close();

    spdlog::debug("Map saved to file successfully");
}

void Map::setTheme(const MapTheme& new_theme)
{
    m_theme = &new_theme;
    ResourceManager::mapThemeBackgroundColor = m_theme->backgroundColor;
    const auto& clear_color = m_theme->backgroundColor;
    glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
}

const MapTheme& Map::getCurrentTheme() const
{
    return *m_theme;
}

void Map::setEndArea(const glm::vec2& pos, const glm::vec2& size)
{
    m_endArea = std::make_unique<OBB::Polygon>(pos, size, 0.f);
}

bool Map::isInEndArea(const glm::vec2& pos, const glm::vec2& size) const
{
    if(m_endArea == nullptr)
    {
        return false;
    }

    return OBB::findCollision(m_endArea->position, m_endArea->size, 0.f, pos, size, 0.f);
}

void Map::update() noexcept { }

void Map::render(
    const glm::mat4& projection,
    const glm::mat4& view,
    bool area,
    bool show_solid,
    bool show_end_area) noexcept
{
    if(show_solid)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    // m_renderer.beginBatch();
    this->drawTiles(area, show_solid);
    this->drawObjects({}, show_solid);
    if(show_end_area)
    {
        this->drawEndArea();
    }
    mono::renderer::render(projection, view);
    if(show_solid)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void Map::drawTiles(bool area, bool show_solid)
{
    spdlog::trace("Drawing Map tiles...");
    if(area)
    {
        mono::renderer::drawQuad(
            {m_centerX, m_centerY},
            {m_width, m_height},
            0.f,
            {0.3f, 0.3f, 0.3f, 1.f});  // background area
    }
    const auto& [wall_block, wall_color, wall_texture] = m_theme->wallBlock;
    for(const auto& tile : m_tiles)
    {
        if(tile.blockId == BlockID::Wall)
        {
            if(wall_texture != nullptr)
            {
                mono::renderer::drawQuad(
                    {tile.x, tile.y},
                    {1.f, 1.f},
                    tile.rotation,
                    wall_texture,
                    wall_color);
            }
            else
            {
                mono::renderer::drawQuad({tile.x, tile.y}, {1.f, 1.f}, tile.rotation, wall_color);
            }
        }
    }
}

void Map::drawObjects(const glm::vec2& hero_pos, bool show_solid)
{
    spdlog::trace("Drawing Map objects...");
    const auto& [wall_block, wall_color, wall_texture] = m_theme->wallBlock;
    for(const auto& object : m_objects)
    {
        const bool col =
            AABB::isColliding(hero_pos, {0.6f, 0.6f}, object.getPosition(), object.getSize());
        glm::vec4 collision_color = {1.f, 1.f, 1.f, col ? object.opacityOnCollision : 1.f};
        // spdlog::trace("OID = {}, collision_color = ({}, {}, {}, {})",
        // objectIdToString(object.id), collision_color.x, collision_color.y, collision_color.z,
        // collision_color.w);
        mono::renderer::drawQuad(
            object.getPosition(),
            object.getSize(),
            object.getRotation(),
            object.getTexture(),
            wall_color * collision_color);
    }
}

void Map::drawEndArea()
{
    if(m_endArea)
    {
        spdlog::trace("Drawing Map end area...");
        const auto& [wall_block, wall_color, wall_texture] = m_theme->wallBlock;
        mono::renderer::drawRect(m_endArea->position, m_endArea->size, 0.f, wall_color);
    }
}

void Map::calculateNewSize(const float& tile_x, const float& tile_y)
{
    const std::size_t center_x = m_width / 2;
    const std::size_t center_y = m_height / 2;
    const auto abs_tile_center_x = std::llroundf(std::abs(tile_x));
    const auto abs_tile_center_y = std::llroundf(std::abs(tile_y));
    if(abs_tile_center_x > center_x)
    {
        m_width = abs_tile_center_x * 2 + 1;
        spdlog::trace(
            "Map: tile.x > center_x ({} > {}): new_width = {}",
            abs_tile_center_x,
            center_x,
            m_width);
        spdlog::debug("Map: Tile is out of bounds, new map width = {}", m_width);
    }
    if(abs_tile_center_y > center_y)
    {
        m_height = abs_tile_center_y * 2 + 1;
        spdlog::trace(
            "Map: tile.y > center_y (|{}| > {}): new_height = {}",
            abs_tile_center_y,
            center_y,
            m_height);
        spdlog::debug("Map: Tile is out of bounds, new map height = {}", m_height);
    }
}
