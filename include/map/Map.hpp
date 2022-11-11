#pragma once

#include "../shader/ShaderManager.hpp"
#include "../texture/Texture.hpp"
#include "MapTheme.hpp"
#include "object/MapObject.hpp"

#include <entt/entity/registry.hpp>

struct Tile
{
    float x = 0;
    float y = 0;
    float rotation = 0.f;
    BlockID block_id;
    bool solid = false;  // indicates if collisions should apply
};

class Map final
{
    public:
    Map(const std::size_t& width, const std::size_t& height);
    ~Map();

    std::size_t getSize() const;
    const std::size_t& getWidth() const;
    const std::size_t& getHeight() const;

    void addObject(const glm::vec2& position, const float& rotation, ObjectID object_id);
    void removeObject(const glm::vec2& position);

    void setTile(const Tile& tile);
    void setTile(const float& x, const float& y, const float& rotation, BlockID block_id, const bool& solid);
    void removeTile(const float& x, const float& y);
    std::vector<Tile>::iterator findTile(const float& x, const float& y);
    void addTilesToRegistry(entt::registry& registry) const;

    void loadFromFile(const std::string& filename);
    void saveToFile(const std::string& filename);

    void setTheme(const MapTheme& new_theme);

    void update() noexcept;
    void render(const glm::mat4& projection, const glm::mat4& view, bool area = false, bool show_solid = false) noexcept;  // one pass draw

    void drawTiles(bool area = false, bool show_solid = false);
    void drawObjects(const glm::vec2& hero_pos, bool show_solid = false);

    protected:
    void calculateNewSize(const float& tile_x, const float& tile_y);

    protected:
    std::size_t m_width = 0;
    std::size_t m_height = 0;
    std::int64_t m_centerX = 0;
    std::int64_t m_centerY = 0;
    std::vector<Tile> m_tiles;
    std::vector<MapObject> m_objects;
    MapTheme& m_theme = MapThemes::FOREST_THEME;
};
