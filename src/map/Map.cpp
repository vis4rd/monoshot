#include "../../include/map/Map.hpp"
#include "../../include/renderer/Renderer.hpp"
#include "../../include/ecs/systems.hpp"
#include "../../include/utility/ResourceManager.hpp"

#include <numeric>

Map::Map(const std::size_t& width, const std::size_t& height)
    : m_width(width),
      m_height(height),
      m_tiles(),
      m_objects()
{
    m_tiles.reserve(width * height);

    this->setTheme(MapThemes::FOREST_THEME);
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
    switch(object_id)
    {
        case Car: break;
        case DestroyedCar: break;
        case Chair: break;
        case OutdoorBench: break;
        case Table: break;
        case SmallTree: break;
        case LargeTree: m_objects.push_back(std::move(MapObject::createLargeTree(position))); break;
        case SmallBush: break;
        case LargeBush: break;
        default: break;
    }
}

void Map::setTile(const Tile& tile)
{
    spdlog::trace("Map: Placing a tile with coords ({}, {}), rotation {}, block_id {}", tile.x, tile.y, tile.rotation, tile.block_id);
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
    spdlog::trace("Map: Finished placing a tile");
}

void Map::setTile(const float& x, const float& y, const float& rotation, BlockID block_id, const bool& solid)
{
    Tile new_tile;
    new_tile.x = std::round(x);
    new_tile.y = std::round(y);
    new_tile.rotation = rotation;
    new_tile.block_id = block_id;
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
    return std::find_if(m_tiles.begin(),
        m_tiles.end(),
        [&x, &y](const Tile tile1)
        {
            return (std::abs(tile1.x - std::round(x)) < 0.1f) && (std::abs(tile1.y - std::round(y)) < 0.1f);
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
        auto& pos = registry.emplace<ecs::component::position>(entity);
        auto& rot = registry.emplace<ecs::component::rotation>(entity);

        pos.x = tile.x;
        pos.y = tile.y;
        rot.data = tile.rotation;
        spdlog::debug("Filling ECS registry with tile pos = ({}, {}), rot = {}", pos.x, pos.y, rot.data);
    }
}

void Map::loadFromFile(const std::string& filename)
{
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

    std::size_t line_count = 0;
    while(file_buffer >> tile_x >> tile_y >> tile_rotation >> tile_block >> tile_solid)
    {
        spdlog::trace("file buffer >> {} >> {} >> {} >> {} >> {}", tile_x, tile_y, tile_rotation, tile_block, tile_solid);
        this->setTile(tile_x, tile_y, tile_rotation, static_cast<BlockID>(tile_block), tile_solid);

        line_count++;
    }
}

void Map::saveToFile(const std::string& filename)
{
    spdlog::debug("Saving map to file '{}'...", filename);

    // check if all tiles are filled
    // if(m_tiles.size() < m_width * m_height)
    // {
    //     spdlog::error("The map is not entirely filled, can not save it to the file '{}'. Ignoring...", filename);
    //     return;
    // }

    // save to the buffer
    std::stringstream file_buffer;
    auto tie = [](const Tile& tile)
    {
        return std::tie(tile.x, tile.y);
    };
    std::sort(m_tiles.begin(),
        m_tiles.end(),
        [&tie](const Tile& tile1, const Tile& tile2)
        {
            return tie(tile1) < tie(tile2);
        });
    for(auto& tile : m_tiles)
    {
        file_buffer << tile.x << ' ' << tile.y << ' ' << tile.rotation << ' ' << tile.block_id << ' ' << tile.solid << '\n';
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
    m_theme = new_theme;
    ResourceManager::mapThemeBackgroundColor = m_theme.backgroundColor;
    const auto& clear_color = m_theme.backgroundColor;
    glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
}

void Map::update() noexcept { }

void Map::render(bool area, bool show_solid) noexcept
{
    spdlog::trace("Rendering map...");
    Renderer::beginBatch();
    if(area)
    {
        Renderer::drawQuad({m_centerX, m_centerY}, {m_width, m_height}, 0.f, {0.3f, 0.3f, 0.3f, 1.f});  // background area
    }
    const auto& [wall_block, wall_color, wall_texture] = m_theme.wallBlock;
    for(const auto& tile : m_tiles)
    {
        if(tile.block_id == BlockID::Wall)
        {
            if(wall_texture != nullptr)
            {
                Renderer::drawQuad({tile.x, tile.y}, {1.f, 1.f}, tile.rotation, wall_texture, wall_color);
            }
            else
            {
                Renderer::drawQuad({tile.x, tile.y}, {1.f, 1.f}, tile.rotation, wall_color);
            }
        }
        if(show_solid && tile.solid)
        {
            Renderer::drawRect({tile.x, tile.y}, {0.2f, 0.2f}, tile.rotation, {1.f, 1.f, 1.f, 1.f});
        }
    }
    for(const auto& object : m_objects)
    {
        Renderer::drawQuad(object.getPosition(), object.getSize(), object.getRotation(), object.getTexture(), wall_color);
    }
    Renderer::endBatch();
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
        spdlog::trace("Map: tile.x > center_x ({} > {}): new_width = {}", abs_tile_center_x, center_x, m_width);
        spdlog::debug("Map: Tile is out of bounds, new map width = {}", m_width);
    }
    if(abs_tile_center_y > center_y)
    {
        m_height = abs_tile_center_y * 2 + 1;
        spdlog::trace("Map: tile.y > center_y (|{}| > {}): new_height = {}", abs_tile_center_y, center_y, m_height);
        spdlog::debug("Map: Tile is out of bounds, new map height = {}", m_height);
    }
}
