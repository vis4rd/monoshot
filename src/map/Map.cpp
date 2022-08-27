#include "../../include/map/Map.hpp"

#include "../../include/renderer/Renderer.hpp"

Map::Map(const std::size_t& width, const std::size_t& height)
    : m_width(width),
      m_height(height),
      m_tiles(width, std::vector<Tile>(height))
{
    std::size_t center_x = m_width / 2;
    std::size_t center_y = m_height / 2;
    for(std::size_t i = 0; i < m_width; i++)
    {
        auto x_diff = static_cast<float>(i) - center_x;
        for(std::size_t j = 0; j < m_height; j++)
        {
            auto y_diff = static_cast<float>(j) - center_y;
            m_tiles[i][j].x = x_diff;
            m_tiles[i][j].y = y_diff;
        }
    }
}

Map::~Map()
{
    spdlog::trace("Deleting Map instance");
    for(auto& texture : m_textures)
    {
        texture.destroy();
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

const std::vector<Texture2D>& Map::getTextures() const
{
    return m_textures;
}

const void Map::setTile(const Tile& tile)
{
    const std::size_t center_x = m_width / 2;
    const std::size_t center_y = m_height / 2;
    // auto x_diff = static_cast<float>(i) - center_x;
    // auto y_diff = static_cast<float>(j) - center_y;

    // x = x_diff
    // y = y_diff
    // i = x_diff + center_x
    // j = y_diff + center_j
    const std::size_t i = static_cast<std::size_t>(tile.x + center_x);
    const std::size_t j = static_cast<std::size_t>(tile.y + center_y);
    m_tiles[i][j] = tile;
}

const void Map::setTile(const float& x, const float& y, const float& rotation, const std::size_t& tex_index, const bool& solid)
{
    const std::size_t center_x = m_width / 2;
    const std::size_t center_y = m_height / 2;
    const std::size_t i = static_cast<std::size_t>(x + center_x);
    const std::size_t j = static_cast<std::size_t>(y + center_y);
    m_tiles[i][j] = {.x = x, .y = y, .rotation = rotation, .textureIndex = tex_index, .solid = solid};
}

void Map::emplaceTexture(const std::int32_t& width, const std::int32_t& height, const std::string& source_path, const std::int32_t& channel_count)
{
    spdlog::trace("Emplacing texture '{}' with size ({}, {})", source_path, width, height);
    auto& texture = m_textures.emplace_back(width, height, channel_count);
    texture.load(source_path);
}

void Map::addTexture(const Texture2D& texture)
{
    m_textures.push_back(texture);
}

void Map::addTexture(Texture2D&& texture)
{
    m_textures.push_back(std::move(texture));
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

    std::size_t i = 0, j = 0;
    while(file_buffer >> tile_x >> tile_y >> tile_rotation >> tile_textureIndex >> tile_solid)
    {
        m_tiles[i][j].rotation = tile_rotation;
        m_tiles[i][j].textureIndex = tile_textureIndex;
        m_tiles[i][j].solid = tile_solid;
        j++;
        if(j == m_height)
        {
            i++;
            j = 0;
            if(i == m_width)
            {
                spdlog::warn("File contains more data than this map can contain");
                break;
            }
        }
    }
}

void Map::update() noexcept { }

void Map::render() noexcept
{
    spdlog::trace("Rendering map...");
    Renderer::beginBatch();
    for(const auto& column : m_tiles)
    {
        for(const auto& tile : column)
        {
            Renderer::drawQuad({tile.x, tile.y}, {1.f, 1.f}, 0.f, m_textures[tile.textureIndex].getID());
        }
    }
    Renderer::endBatch();
}
