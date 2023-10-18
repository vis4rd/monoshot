#include "../../include/gl/BufferElement.hpp"

#include <spdlog/spdlog.h>

#include "../../include/gl/ShaderDataType.hpp"

namespace mono
{

inline static std::uint32_t sizeofShaderDataType(const ShaderDataType& type)
{
    switch(type)
    {
        case ShaderDataType::BOOL1: return 1;
        case ShaderDataType::FLOAT1:
        case ShaderDataType::INT1: return 4;
        case ShaderDataType::FLOAT2:
        case ShaderDataType::INT2: return 4 * 2;
        case ShaderDataType::FLOAT3:
        case ShaderDataType::INT3: return 4 * 3;
        case ShaderDataType::FLOAT4:
        case ShaderDataType::INT4: return 4 * 4;
        case ShaderDataType::MAT3: return 4 * 3 * 3;
        case ShaderDataType::MAT4: return 4 * 4 * 4;
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
        case ShaderDataType::BOOL1:
        case ShaderDataType::FLOAT1:
        case ShaderDataType::INT1: return 1;
        case ShaderDataType::FLOAT2:
        case ShaderDataType::INT2: return 2;
        case ShaderDataType::FLOAT3:
        case ShaderDataType::INT3:
        case ShaderDataType::MAT3: return 3;
        case ShaderDataType::FLOAT4:
        case ShaderDataType::INT4:
        case ShaderDataType::MAT4: return 4;
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
