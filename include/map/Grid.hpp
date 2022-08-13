#pragma once

#include "../shader/ShaderManager.hpp"

template<std::size_t WIDTH, std::size_t HEIGHT>
class Grid
{
    public:
    using tile_t = std::uint8_t;

    explicit Grid(const std::array<tile_t, WIDTH * HEIGHT>& tiles);
    Grid(const std::initializer_list<tile_t>& tiles);
    virtual ~Grid() = default;

    virtual const tile_t* data() const final;

    auto operator[](const std::size_t& index);
    const auto operator[](const std::size_t& index) const;
    virtual tile_t& at(const std::size_t& index) final;
    virtual const tile_t& at(const std::size_t& index) const final;

    virtual void prepareForRender();
    virtual void update() noexcept;
    virtual void render() noexcept;

    protected:
    tile_t m_tiles[WIDTH][HEIGHT] = {0u};
    std::uint32_t m_vao;
    std::uint32_t m_vbo;
    std::uint32_t m_ebo;
    std::vector<float> m_vertexBuffer;
    std::vector<std::uint32_t> m_elementBuffer;
};

#include "../../src/map/Grid.inl"
