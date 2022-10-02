#include "../../include/map/Map.hpp"
#include "../../include/renderer/Renderer.hpp"
#include "../../include/ecs/systems.hpp"

#include <numeric>

Map::Map(const std::size_t& width, const std::size_t& height)
    : m_width(width),
      m_height(height),
      m_tiles()
{
    m_tiles.reserve(width * height);

    auto& tex_ptr = m_textures.emplace_back(std::make_shared<Texture2D>(1, 1));
    std::uint32_t tex = 0x00000000;
    tex_ptr->load(reinterpret_cast<std::uint8_t*>(&tex), sizeof(std::uint32_t));
}

Map::~Map()
{
    spdlog::trace("Deleting Map instance");
    for(auto& texture : m_textures)
    {
        texture->destroy();
    }
    spdlog::trace("Map deletion complete");
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

const std::vector<std::shared_ptr<Texture2D>>& Map::getTextures() const
{
    return m_textures;
}

void Map::setTile(const Tile& tile)
{
    spdlog::trace("Map: Placing a tile with coords ({}, {}), rotation {}, tex_index {}", tile.x, tile.y, tile.rotation, tile.textureIndex);
    this->calculateNewSize(tile.x, tile.y);

    m_tiles.push_back(tile);
    spdlog::trace("Map: Finished placing a tile");
}

void Map::setTile(const float& x, const float& y, const float& rotation, const std::size_t& tex_index, const bool& solid)
{
    Tile new_tile;
    new_tile.x = std::round(x);
    new_tile.y = std::round(y);
    new_tile.rotation = rotation;
    new_tile.textureIndex = tex_index;
    new_tile.solid = solid;
    this->setTile(new_tile);
}

void Map::removeTile(const float& x, const float& y)
{
    m_tiles.erase(std::remove_if(m_tiles.begin(),
                      m_tiles.end(),
                      [&x, &y](const Tile tile1)
                      {
                          return (std::abs(tile1.x - std::round(x)) < 0.1f) && (std::abs(tile1.y - std::round(y)) < 0.1f);
                      }),
        m_tiles.end());
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

void Map::emplaceTexture(const std::int32_t& width, const std::int32_t& height, const std::string& source_path, const std::int32_t& channel_count)
{
    spdlog::trace("Emplacing texture '{}' with size ({}, {})", source_path, width, height);
    auto texture_ptr = std::make_shared<Texture2D>(width, height, channel_count);
    texture_ptr->load(source_path);
    m_textures.push_back(std::move(texture_ptr));
}

void Map::addTexture(const Texture2D& texture)
{
    m_textures.push_back(std::make_shared<Texture2D>(texture));
}

void Map::addTexture(Texture2D&& texture)
{
    m_textures.push_back(std::move(std::make_shared<Texture2D>(texture)));
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
    std::size_t tile_textureIndex = 0;
    bool tile_solid = false;

    std::size_t line_count = 0;
    while(file_buffer >> tile_x >> tile_y >> tile_rotation >> tile_textureIndex >> tile_solid)
    {
        spdlog::trace("file buffer >> {} >> {} >> {} >> {} >> {}", tile_x, tile_y, tile_rotation, tile_textureIndex, tile_solid);
        this->setTile(tile_x, tile_y, tile_rotation, tile_textureIndex, tile_solid);

        line_count++;
    }
}

void Map::saveToFile(const std::string& filename)
{
    spdlog::debug("Saving map to file '{}'...", filename);

    // check if all tiles are filled
    if(m_tiles.size() < m_width * m_height)
    {
        spdlog::error("The map is not entirely filled, can not save it to the file '{}'. Ignoring...", filename);
        return;
    }

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
        file_buffer << tile.x << ' ' << tile.y << ' ' << tile.rotation << ' ' << tile.textureIndex << ' ' << tile.solid << '\n';
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

void Map::update() noexcept { }

void Map::render(bool area, bool show_solid) noexcept
{
    spdlog::trace("Rendering map...");
    Renderer::beginBatch();
    if(area)
    {
        Renderer::drawQuad({m_centerX, m_centerY}, {m_width, m_height}, 0.f, {0.3f, 0.3f, 0.3f, 1.f});  // background area
    }
    for(const auto& tile : m_tiles)
    {
        Renderer::drawQuad({tile.x, tile.y}, {1.f, 1.f}, tile.rotation, m_textures[tile.textureIndex]);
        if(show_solid && tile.solid)
        {
            Renderer::drawRect({tile.x, tile.y}, {0.2f, 0.2f}, tile.rotation, {1.f, 1.f, 1.f, 1.f});
        }
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
