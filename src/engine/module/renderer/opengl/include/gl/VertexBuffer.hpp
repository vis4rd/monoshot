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
    explicit VertexBuffer(std::uint32_t size);
    VertexBuffer(const float* vertices, std::uint32_t size);
    VertexBuffer(const VertexBuffer& copy);
    VertexBuffer(VertexBuffer&& move) noexcept;
    ~VertexBuffer();

    VertexBuffer& operator=(const VertexBuffer& copy) = default;
    VertexBuffer& operator=(VertexBuffer&& move) noexcept;

    void bind() const;
    void unbind() const;

    const std::uint32_t& getID() const;
    const BufferLayout& getLayout() const;

    void setData(const void* data, const std::uint32_t& size);
    void setLayout(const BufferLayout& layout);

    // NOLINTNEXTLINE(google-explicit-constructor)
    operator std::uint32_t() const;

    private:
    std::uint32_t m_id{};
    BufferLayout m_layout{};
};

}  // namespace mono
