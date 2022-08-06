#include "../../include/utility/VertexArray.hpp"

VertexArray::VertexArray()
{
    glCreateVertexArrays(1, &m_id);
}

const VertexArray::operator std::uint32_t() const
{
    return m_id;
}
