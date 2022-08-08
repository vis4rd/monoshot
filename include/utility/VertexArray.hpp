#pragma once

#include <memory>

#include "VertexBuffer.hpp"
#include "ElementBuffer.hpp"

class VertexArray
{
    public:
    VertexArray();
    ~VertexArray();

    void bind() const;
    void unbind() const;

    void addVertexBuffer(VertexBuffer&& vertex_buffer);
    void addElementBuffer(const ElementBuffer& element_buffer);

    const std::vector<VertexBuffer>& getVertexBuffers() const;
    const std::unique_ptr<ElementBuffer>& getElementBuffer() const;

    operator std::uint32_t();

    private:
    std::uint32_t m_id = 0u;
    std::uint32_t m_vertexBufferIndex = 0u;
    std::vector<VertexBuffer> m_vertexBuffers;
    std::unique_ptr<ElementBuffer> m_elementBuffer;
};
