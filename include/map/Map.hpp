#pragma once

#include "../shader/ShaderManager.hpp"
#include "../texture/Texture2D.hpp"
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

class Map
{
    public:
    Map(const std::size_t& width, const std::size_t& height);
    virtual ~Map();

    virtual std::size_t getSize() const final;
    virtual const std::size_t& getWidth() const final;
    virtual const std::size_t& getHeight() const final;

    virtual void addObject(const glm::vec2& position, const float& rotation, ObjectID object_id) final;

    virtual void setTile(const Tile& tile) final;
    virtual void setTile(const float& x, const float& y, const float& rotation, BlockID block_id, const bool& solid) final;
    virtual void removeTile(const float& x, const float& y) final;
    virtual std::vector<Tile>::iterator findTile(const float& x, const float& y) final;
    virtual void addTilesToRegistry(entt::registry& registry) const final;

    void loadFromFile(const std::string& filename);
    void saveToFile(const std::string& filename);

    virtual void setTheme(const MapTheme& new_theme) final;

    virtual void update() noexcept;
    virtual void render(bool area = false, bool show_solid = false) noexcept;

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
