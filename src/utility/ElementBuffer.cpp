#include "../../include/utility/ElementBuffer.hpp"
#include "spdlog/spdlog.h"
#include <glad/glad.h>

ElementBuffer::ElementBuffer(std::uint32_t* indices, std::uint32_t count)
    : m_count(count)
{
    spdlog::debug("Creating ElementBuffer instance");
    glCreateBuffers(1, &m_id);
    glNamedBufferData(m_id, m_count * sizeof(std::uint32_t), indices, GL_STATIC_DRAW);
}

ElementBuffer::~ElementBuffer()
{
    spdlog::debug("Deleting ElementBuffer instance");
    glDeleteBuffers(1, &m_id);
}

void ElementBuffer::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void ElementBuffer::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

std::uint32_t ElementBuffer::getElementCount() const
{
    return m_count;
}

ElementBuffer::operator std::uint32_t() const
{
    return m_id;
}
