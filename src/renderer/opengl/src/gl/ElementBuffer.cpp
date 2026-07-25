#include "../../include/opengl/gl/ElementBuffer.hpp"

namespace mono::gl
{

ElementBuffer::ElementBuffer(const std::uint32_t* indices, std::uint32_t count)
    : m_count(count)
{
    ::gl::glCreateBuffers(1, &m_id);
    ::gl::glNamedBufferData(
        m_id,
        static_cast<::gl::GLsizeiptr>(m_count * sizeof(std::uint32_t)),
        indices,
        ::gl::GL_STATIC_DRAW);
    log::setGlObjectLabel(::gl::GL_BUFFER, m_id, "ElementBuffer#{}", m_id);
    spdlog::trace("Created ElementBuffer instance with ID = {} and count = {}", m_id, m_count);
}

ElementBuffer::~ElementBuffer()
{
    spdlog::trace("Calling destructor of ElementBuffer instance with ID = {} (no destroy)", m_id);
}

void ElementBuffer::bind() const
{
    // spdlog::trace("Binding ElementBuffer with ID = {}", m_id);
    ::gl::glBindBuffer(::gl::GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void ElementBuffer::unbind() const
{
    ::gl::glBindBuffer(::gl::GL_ELEMENT_ARRAY_BUFFER, 0);
}

const ::gl::GLuint& ElementBuffer::getID() const
{
    return m_id;
}

std::uint32_t ElementBuffer::getElementCount() const
{
    return m_count;
}

ElementBuffer::operator ::gl::GLuint() const
{
    return m_id;
}

}  // namespace mono::gl
