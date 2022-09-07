#pragma once

#include "../shader/ShaderManager.hpp"
#include "../texture/Texture2D.hpp"

struct Tile
{
    float x = 0;
    float y = 0;
    float rotation = 0.f;
    std::size_t textureIndex = 0;
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
    virtual const std::vector<std::shared_ptr<Texture2D>>& getTextures() const final;

    virtual const void setTile(const Tile& tile) final;
    virtual const void setTile(const float& x, const float& y, const float& rotation, const std::size_t& tex_index = 0, const bool& solid = false) final;

    virtual void emplaceTexture(const std::int32_t& width, const std::int32_t& height, const std::string& source_path, const std::int32_t& channel_count = 0) final;
    virtual void addTexture(const Texture2D& texture) final;
    virtual void addTexture(Texture2D&& texture) final;

    void loadFromFile(const std::string& filename);
    void saveToFile(const std::string& filename);

    virtual void update() noexcept;
    virtual void render(bool area = false) noexcept;

    protected:
    void calculateNewSize(const float& tile_x, const float& tile_y);

    protected:
    std::size_t m_width = 0;
    std::size_t m_height = 0;
    std::int64_t m_centerX = 0;
    std::int64_t m_centerY = 0;
    std::vector<Tile> m_tiles;
    std::vector<std::shared_ptr<Texture2D>> m_textures;
};
