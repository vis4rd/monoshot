#pragma once

#include "../shader/ShaderManager.hpp"

class Grid
{
    public:
    Grid(const std::size_t& width, const std::size_t& height);
    Grid(const std::initializer_list<std::uint8_t>& tiles);
    virtual ~Grid() = default;

    virtual const std::uint8_t* data() const final;

    virtual std::vector<std::uint8_t>& operator[](const std::size_t& index) final;
    virtual const std::vector<std::uint8_t>& operator[](const std::size_t& index) const final;
    virtual std::vector<std::uint8_t>& at(const std::size_t& index) final;
    virtual const std::vector<std::uint8_t>& at(const std::size_t& index) const final;
    virtual std::uint8_t& at(const std::size_t& index_x, const std::size_t& index_y) final;
    virtual const std::uint8_t& at(const std::size_t& index_x, const std::size_t& index_y) const final;

    virtual void prepareForRender();
    virtual void update() noexcept;
    virtual void render() noexcept;

    protected:
    std::size_t m_width = 0;
    std::size_t m_height = 0;
    std::vector<std::vector<std::uint8_t>> m_tiles;

    std::uint32_t m_vao;
    std::uint32_t m_vbo;
    std::uint32_t m_ebo;
    std::vector<float> m_vertexBuffer;
    std::vector<std::uint32_t> m_elementBuffer;
};
