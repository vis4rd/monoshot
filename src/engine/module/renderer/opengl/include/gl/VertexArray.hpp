#pragma once

#include <memory>

#include "ElementBuffer.hpp"
#include "VertexBuffer.hpp"

namespace mono
{

class VertexArray
{
    public:
    VertexArray();
    VertexArray(const VertexArray& copy) = default;
    VertexArray(VertexArray&& move) noexcept;
    ~VertexArray();

    VertexArray& operator=(const VertexArray& copy) = default;
    VertexArray& operator=(VertexArray&& move) noexcept;

    void bind() const;
    void unbind() const;

    void addVertexBuffer(VertexBuffer&& vertex_buffer);
    void addElementBuffer(const ElementBuffer& element_buffer);

    std::vector<VertexBuffer>& getVertexBuffers();
    const std::vector<VertexBuffer>& getVertexBuffers() const;
    const ElementBuffer& getElementBuffer() const;

    // NOLINTNEXTLINE(google-explicit-constructor)
    operator std::uint32_t();

    private:
    std::uint32_t m_id = 0u;
    std::uint32_t m_vertexBufferIndex = 0u;
    std::vector<VertexBuffer> m_vertexBuffers{};
    ElementBuffer m_elementBuffer{};
};

}  // namespace mono
