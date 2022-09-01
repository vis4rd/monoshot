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
    spdlog::debug("Map: Placing a tile with coords ({}, {}), rotation {}, tex_index {}", x, y, rotation, tex_index);
    const float center_x = m_width / 2.f;
    const float center_y = m_height / 2.f;
    if((x < -center_x) || (y < -center_y) || (x > center_x) || (y > center_y))
    {
        spdlog::error("Map: Trying to place a tile outside of bounds! (|x| < {} or |y| < {})", center_x, center_y);
        return;
    }
    const std::size_t i = std::llroundf(x + center_x - 0.5f);
    const std::size_t j = std::llroundf(y + center_y - 0.5f);
    const float xx = std::round(x);
    const float yy = std::round(y);
    m_tiles[i][j].x = xx;
    m_tiles[i][j].y = yy;
    m_tiles[i][j].rotation = rotation;
    m_tiles[i][j].textureIndex = tex_index;
    m_tiles[i][j].solid = solid;
    spdlog::debug("Map: Finished placing a tile");
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

    std::size_t i = 0, j = 0;
    std::size_t line_count = 0;
    while(file_buffer >> tile_x >> tile_y >> tile_rotation >> tile_textureIndex >> tile_solid)
    {
        if(line_count > m_width * m_height)
        {
            spdlog::warn("File '{}' contains more data than this map can contain", filename);
            break;
        }
        spdlog::debug("file buffer >> {} >> {} >> {} >> {} >> {}", tile_x, tile_y, tile_rotation, tile_textureIndex, tile_solid);
        const auto i = line_count % m_width;
        const auto j = line_count / m_width;
        m_tiles[i][j].rotation = tile_rotation;
        m_tiles[i][j].textureIndex = tile_textureIndex;
        m_tiles[i][j].solid = tile_solid;

        line_count++;
    }
}

void Map::saveToFile(const std::string& filename)
{
    spdlog::debug("Saving map to file '{}'...", filename);

    // check if all tiles are filled
    bool okay = true;
    for(std::size_t i = 0; i < m_width; i++)
    {
        for(std::size_t j = 0; j < m_height; j++)
        {
            if(m_tiles[i][j].textureIndex < 1)
            {
                okay = false;
                spdlog::error("The tile ({}, {}) is not filled...", i, j);
            }
        }
    }
    if(!okay)
    {
        spdlog::error("The map is not entirely filled, can not save it to the file '{}'. Ignoring...", filename);
        return;
    }

    // save to the buffer
    std::stringstream file_buffer;
    for(std::size_t i = 0; i < m_width; i++)
    {
        for(std::size_t j = 0; j < m_height; j++)
        {
            const auto& t = m_tiles[i][j];  // convenience abbreviation
            file_buffer << t.x << ' ' << t.y << ' ' << t.rotation << ' ' << t.textureIndex << ' ' << t.solid << '\n';
        }
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

void Map::render(bool area) noexcept
{
    spdlog::trace("Rendering map...");
    Renderer::beginBatch();
    if(area)
    {
        Renderer::drawQuad({0.f, 0.f}, {m_width, m_height}, 0.f, {0.3f, 0.3f, 0.3f, 1.f});  // background area
    }
    for(const auto& column : m_tiles)
    {
        for(const auto& tile : column)
        {
            Renderer::drawQuad({tile.x, tile.y}, {1.f, 1.f}, 0.f, m_textures[tile.textureIndex]->getID());
        }
    }
    Renderer::endBatch();
}
