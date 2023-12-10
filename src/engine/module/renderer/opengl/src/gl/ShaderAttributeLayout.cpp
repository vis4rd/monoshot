#include "../../include/gl/ShaderAttributeLayout.hpp"

namespace mono::gl
{

ShaderAttributeLayout::ShaderAttributeLayout(std::initializer_list<ShaderAttribute> attributes)
    : m_attributes(attributes)
{
    // for(std::int32_t i = 0; i < m_attributes.size(); i++)
    // {
    //     const auto& element = m_attributes.at(i);
    //     ShaderDataType target_data_type{};
    //     std::int32_t target_attribute_count{};
    //     if(element.getShaderDataType() == ShaderDataType::MAT3)
    //     {
    //         target_data_type = ShaderDataType::FLOAT3;
    //         target_attribute_count = 3;
    //     }
    //     else if(element.getShaderDataType() == ShaderDataType::MAT4)
    //     {
    //         target_data_type = ShaderDataType::FLOAT4;
    //         target_attribute_count = 4;
    //     }
    //     for(std::int32_t j = 0; j < target_attribute_count; j++)
    //     {
    //         m_attributes.emplace(
    //             m_attributes.begin() + i + j + 1,
    //             target_data_type,
    //             element.getName(),
    //             false,
    //             element.getUpdateFrequency(),
    //             element.getOffset() + sizeof(float) * target_attribute_count * j);
    //     }
    //     m_attributes.erase(m_attributes.begin() + i);
    // }
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
        offset += attribute.getSize();
        m_stride += attribute.getSize();
    }
}

}  // namespace mono::gl
