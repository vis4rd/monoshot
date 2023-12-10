#include "../../include/gl/ElementBuffer.hpp"

#include <glad/gl.h>
#include <spdlog/spdlog.h>

namespace mono::gl
{

ElementBuffer::ElementBuffer(const std::uint32_t* indices, std::uint32_t count)
    : m_count(count)
    , m_isInit(true)
{
    glCreateBuffers(1, &m_id);
    glNamedBufferData(
        m_id,
        static_cast<std::int64_t>(m_count * sizeof(std::uint32_t)),
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
    if(m_isInit)
    {
        spdlog::trace("Binding ElementBuffer with ID = {}", m_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    }
}

void ElementBuffer::unbind() const
{
    if(m_isInit)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}

const std::uint32_t& ElementBuffer::getID() const
{
    return m_id;
}

std::uint32_t ElementBuffer::getElementCount() const
{
    if(m_isInit)
    {
        return m_count;
    }
    else
    {
        return 0;
    }
}

void ElementBuffer::setData(std::uint32_t* indices, std::uint32_t count)
{
    m_isInit = true;
    glCreateBuffers(1, &m_id);
    glNamedBufferData(
        m_id,
        static_cast<std::int64_t>(m_count * sizeof(std::uint32_t)),
        indices,
        GL_STATIC_DRAW);
}

bool ElementBuffer::isInitialized() const
{
    return m_isInit;
}

ElementBuffer::operator std::uint32_t() const
{
    if(m_isInit)
    {
        return m_id;
    }
    else
    {
        return 0;
    }
}

}  // namespace mono::gl
