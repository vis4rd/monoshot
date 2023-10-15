#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "BufferLayout.hpp"

namespace mono
{

class VertexBuffer
{
    public:
    VertexBuffer(std::uint32_t size);
    VertexBuffer(const float* vertices, std::uint32_t size);
    VertexBuffer(const VertexBuffer& copy);
    VertexBuffer(VertexBuffer&& move) noexcept;
    ~VertexBuffer();

    void bind() const;
    void unbind() const;

    const std::uint32_t& getID() const;
    const BufferLayout& getLayout() const;

    void setData(const void* data, const std::uint32_t& size);
    void setLayout(const BufferLayout& layout);

    operator std::uint32_t() const;

    private:
    std::uint32_t m_id;
    BufferLayout m_layout;
};

}  // namespace mono
