#include "../../include/gl/ElementBuffer.hpp"

namespace mono::gl
{

ElementBuffer::ElementBuffer(const std::uint32_t* indices, std::uint32_t count)
    : m_count(count)
{
    glCreateBuffers(1, &m_id);
    glNamedBufferData(
        m_id,
        static_cast<GLsizeiptr>(m_count * sizeof(std::uint32_t)),
        indices,
        GL_STATIC_DRAW);
    spdlog::debug("Created ElementBuffer instance with ID = {} and count = {}", m_id, m_count);
}

ElementBuffer::~ElementBuffer()
{
    spdlog::debug("Calling destructor of ElementBuffer instance with ID = {} (no destroy)", m_id);
}

void ElementBuffer::bind() const
{
    spdlog::trace("Binding ElementBuffer with ID = {}", m_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void ElementBuffer::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

const GLuint& ElementBuffer::getID() const
{
    return m_id;
}

std::uint32_t ElementBuffer::getElementCount() const
{
    return m_count;
}

ElementBuffer::operator GLuint() const
{
    return m_id;
}

}  // namespace mono::gl
