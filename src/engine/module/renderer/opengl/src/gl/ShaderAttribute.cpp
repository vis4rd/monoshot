#include "../../include/gl/ShaderAttribute.hpp"

#include <spdlog/spdlog.h>

namespace mono::gl
{

ShaderAttribute::ShaderAttribute(const ShaderAttributeTypeInfo& type, const std::string& name)
    : m_name(name)
    , m_shaderType(type)
{ }

ShaderAttribute::ShaderAttribute(
    const ShaderAttributeTypeInfo& type,
    const std::string& name,
    bool normalized,
    std::size_t offset)
    : m_name(name)
    , m_shaderType(type)
    , m_offset(offset)
    , m_normalized(normalized)
{ }

ShaderAttribute::ShaderAttribute(
    const ShaderAttributeTypeInfo& type,
    const std::string& name,
    std::size_t offset,
    bool normalized)
    : ShaderAttribute(type, name, normalized, offset)
{ }

const std::string& ShaderAttribute::getName() const
{
    return m_name;
}

const ShaderAttributeTypeInfo& ShaderAttribute::getType() const
{
    return m_shaderType;
}

std::uint32_t ShaderAttribute::getBytesize() const
{
    return m_shaderType.bytesize;
}

std::size_t ShaderAttribute::getOffset() const
{
    return m_offset;
}

bool ShaderAttribute::isNormalized() const
{
    return m_normalized;
}

std::uint32_t ShaderAttribute::getComponentCount() const
{
    return m_shaderType.componentCount;
}

void ShaderAttribute::setOffset(const std::size_t& offset)
{
    m_offset = offset;
}

}  // namespace mono::gl
