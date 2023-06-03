#pragma once

#include <memory>

#include "ElementBuffer.hpp"
#include "VertexBuffer.hpp"

class VertexArray
{
    public:
    VertexArray();
    VertexArray(const VertexArray& copy);
    VertexArray(VertexArray&& move);
    ~VertexArray();

    void bind() const;
    void unbind() const;

    void addVertexBuffer(VertexBuffer&& vertex_buffer);
    void addElementBuffer(const ElementBuffer& element_buffer);

    std::vector<VertexBuffer>& getVertexBuffers();
    const std::vector<VertexBuffer>& getVertexBuffers() const;
    const ElementBuffer& getElementBuffer() const;

    operator std::uint32_t();

    private:
    std::uint32_t m_id = 0u;
    std::uint32_t m_vertexBufferIndex = 0u;
    std::vector<VertexBuffer> m_vertexBuffers{};
    ElementBuffer m_elementBuffer{};
};