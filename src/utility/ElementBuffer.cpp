#include "../../include/utility/ElementBuffer.hpp"
#include "spdlog/spdlog.h"
#include <glad/glad.h>

ElementBuffer::ElementBuffer(std::uint32_t* indices, std::uint32_t count)
    : m_count(count),
      m_isInit(true)
{
    spdlog::debug("Creating ElementBuffer instance");
    glCreateBuffers(1, &m_id);
    glNamedBufferData(m_id, m_count * sizeof(std::uint32_t), indices, GL_STATIC_DRAW);
}

ElementBuffer::~ElementBuffer()
{
    spdlog::debug("Deleting ElementBuffer instance");
}

void ElementBuffer::bind() const
{
    if(m_isInit)
    {
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
    glNamedBufferData(m_id, m_count * sizeof(std::uint32_t), indices, GL_STATIC_DRAW);
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
