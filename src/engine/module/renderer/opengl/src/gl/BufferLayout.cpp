#include "../../include/gl/BufferLayout.hpp"

namespace mono
{

BufferLayout::BufferLayout(std::initializer_list<BufferElement> elements)
    : m_elements(elements)
{
    this->calculateOffsetAndStride();
}

std::uint32_t BufferLayout::getStride() const
{
    return m_stride;
}

const std::vector<BufferElement>& BufferLayout::getElements() const
{
    return m_elements;
}

std::vector<BufferElement>::iterator BufferLayout::begin()
{
    return m_elements.begin();
}

std::vector<BufferElement>::iterator BufferLayout::end()
{
    return m_elements.end();
}

std::vector<BufferElement>::const_iterator BufferLayout::begin() const
{
    return m_elements.begin();
}

std::vector<BufferElement>::const_iterator BufferLayout::end() const
{
    return m_elements.end();
}

void BufferLayout::calculateOffsetAndStride()
{
    std::size_t offset = 0;
    m_stride = 0;
    for(auto& element : m_elements)
    {
        element.setOffset(offset);
        offset += element.getSize();
        m_stride += element.getSize();
    }
}

}  // namespace mono
