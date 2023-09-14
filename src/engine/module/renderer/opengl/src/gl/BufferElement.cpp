#include "../../include/gl/BufferElement.hpp"

#include <spdlog/spdlog.h>

#include "../../include/gl/ShaderDataType.hpp"

namespace mono
{

inline static std::uint32_t sizeofShaderDataType(const ShaderDataType& type)
{
    switch(type)
    {
        case ShaderDataType::bool1: return 1;
        case ShaderDataType::float1:
        case ShaderDataType::int1: return 4;
        case ShaderDataType::float2:
        case ShaderDataType::int2: return 4 * 2;
        case ShaderDataType::float3:
        case ShaderDataType::int3: return 4 * 3;
        case ShaderDataType::float4:
        case ShaderDataType::int4: return 4 * 4;
        case ShaderDataType::mat3: return 4 * 3 * 3;
        case ShaderDataType::mat4: return 4 * 4 * 4;
        default:
        {
            spdlog::error("Unknown ShaderDataType, returning 0");
            return 0;
        }
    }
}

BufferElement::BufferElement(const ShaderDataType& type, const std::string& name, bool normalized)
    : m_name(name)
    , m_shaderType(type)
    , m_size(sizeofShaderDataType(type))
    , m_offset(0)
    , m_normalized(normalized)
{ }

const std::string& BufferElement::getName() const
{
    return m_name;
}

const ShaderDataType& BufferElement::getShaderDataType() const
{
    return m_shaderType;
}

std::uint32_t BufferElement::getSize() const
{
    return m_size;
}

std::size_t BufferElement::getOffset() const
{
    return m_offset;
}

bool BufferElement::isNormalized() const
{
    return m_normalized;
}

std::uint32_t BufferElement::getComponentCount() const
{
    switch(m_shaderType)
    {
        case ShaderDataType::bool1:
        case ShaderDataType::float1:
        case ShaderDataType::int1: return 1;
        case ShaderDataType::float2:
        case ShaderDataType::int2: return 2;
        case ShaderDataType::float3:
        case ShaderDataType::int3:
        case ShaderDataType::mat3: return 3;
        case ShaderDataType::float4:
        case ShaderDataType::int4:
        case ShaderDataType::mat4: return 4;
        default:
        {
            spdlog::error("Unknown ShaderDataType, returning 0");
            return 0;
        }
    }
}

void BufferElement::setSize(const std::uint32_t& size)
{
    m_size = size;
}

void BufferElement::setOffset(const std::size_t& offset)
{
    m_offset = offset;
}

}  // namespace mono
