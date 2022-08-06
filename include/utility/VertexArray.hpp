#pragma once

#include <glad/glad.h>

class VertexArray
{
    public:
    VertexArray();
    ~VertexArray() = default;

    void bind() const;
    void unbind() const;

    // void addVertexBuffer(const VertexBuffer& vertex_buffer);
    // void addElementBuffer(const ElementBuffer& element_buffer);

    // const std::vector<VertexBuffer>& getVertexBuffers() const;
    // const ElementBuffer& getElementBuffer() const;

    const operator std::uint32_t() const;

    private:
    std::uint32_t m_id = 0u;
    // std::vector<VertexBuffer> m_vertexBuffers;
    // ElementBuffer m_elementBuffer;
};
