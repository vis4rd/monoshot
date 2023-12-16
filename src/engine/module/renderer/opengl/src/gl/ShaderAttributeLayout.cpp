#include "../../include/gl/ShaderAttributeLayout.hpp"

namespace mono::gl
{

ShaderAttributeLayout::ShaderAttributeLayout(std::initializer_list<ShaderAttribute> attributes)
    : m_attributes(attributes)
{
    this->calculateOffsetAndStride();
}

std::uint32_t ShaderAttributeLayout::getStride() const
{
    return m_stride;
}

std::vector<ShaderAttribute>& ShaderAttributeLayout::getAttributes()
{
    return m_attributes;
}

std::vector<ShaderAttribute>::iterator ShaderAttributeLayout::begin()
{
    return m_attributes.begin();
}

std::vector<ShaderAttribute>::iterator ShaderAttributeLayout::end()
{
    return m_attributes.end();
}

std::vector<ShaderAttribute>::const_iterator ShaderAttributeLayout::begin() const
{
    return m_attributes.begin();
}

std::vector<ShaderAttribute>::const_iterator ShaderAttributeLayout::end() const
{
    return m_attributes.end();
}

void ShaderAttributeLayout::calculateOffsetAndStride()
{
    std::size_t offset = 0;
    for(auto& attribute : m_attributes)
    {
        attribute.setOffset(offset);
        offset += attribute.getBytesize();
        m_stride += attribute.getBytesize();
    }
}

}  // namespace mono::gl
