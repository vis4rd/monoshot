#include "../../include/gl/VertexBuffer.hpp"

#include <glad/gl.h>
#include <spdlog/spdlog.h>

namespace mono
{

VertexBuffer::VertexBuffer(std::uint32_t size)
{
    glCreateBuffers(1, &m_id);
    glNamedBufferData(m_id, size, nullptr, GL_DYNAMIC_DRAW);
    spdlog::debug("Created VertexBuffer instance with ID = {} and size = {}", m_id, size);
}

VertexBuffer::VertexBuffer(const VertexBuffer& copy)
    : m_id(copy.m_id)
    , m_layout(copy.m_layout)
{
    spdlog::debug("Copying VertexBuffer instance with ID = {}", copy.m_id);
}

VertexBuffer::VertexBuffer(VertexBuffer&& move)
    : m_id(std::move(move.m_id))
    , m_layout(std::move(move.m_layout))
{
    spdlog::debug("Moving VertexBuffer instance with ID = {}", m_id);
}

VertexBuffer::VertexBuffer(const float* vertices, std::uint32_t size)
{
    spdlog::debug("Creating VertexBuffer...");
    glCreateBuffers(1, &m_id);
    glNamedBufferData(m_id, size, vertices, GL_STATIC_DRAW);
    for(int i = 0; i < size / sizeof(float); i++)
    {
        spdlog::trace("Vertex {} = {}", i, vertices[i]);
    }
    spdlog::debug(
        "Created VertexBuffer instance with ID = {}, size = {} and pre-computed vertices",
        m_id,
        size);
}

VertexBuffer::~VertexBuffer()
{
    spdlog::debug("Calling destructor of VertexBuffer with ID = {} (no destroy)", m_id);
}

void VertexBuffer::bind() const
{
    spdlog::trace("Binding VertexBuffer with ID = {}", m_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void VertexBuffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

const std::uint32_t& VertexBuffer::getID() const
{
    return m_id;
}

const BufferLayout& VertexBuffer::getLayout() const
{
    return m_layout;
}

void VertexBuffer::setData(const void* data, const std::uint32_t& size)
{
    spdlog::trace("Setting drawing data to VertexBuffer with ID = {}", m_id);
    glNamedBufferSubData(m_id, 0, size, data);
}

void VertexBuffer::setLayout(const BufferLayout& layout)
{
    m_layout = layout;
}

VertexBuffer::operator std::uint32_t() const
{
    return m_id;
}

}  // namespace mono
