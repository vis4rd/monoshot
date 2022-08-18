#include "../../include/utility/VertexBuffer.hpp"
#include "spdlog/spdlog.h"
#include <glad/glad.h>

BufferLayout::BufferLayout(std::initializer_list<BufferElement> elements)
    : m_elements(elements)
{
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

///////////////////////////////////////////////////////////////////////////////

VertexBuffer::VertexBuffer(std::uint32_t size)
{
    spdlog::debug("Creating VertexBuffer instance with size {}", size);
    glCreateBuffers(1, &m_id);
    glNamedBufferData(m_id, size, nullptr, GL_DYNAMIC_DRAW);
}

VertexBuffer::VertexBuffer(const VertexBuffer& copy)
    : m_id(copy.m_id),
      m_layout(copy.m_layout)
{
}

VertexBuffer::VertexBuffer(VertexBuffer&& move)
    : m_id(std::move(move.m_id)),
      m_layout(std::move(move.m_layout))
{
}

VertexBuffer::VertexBuffer(const float* vertices, std::uint32_t size)
{
    spdlog::debug("Creating VertexBuffer instance with size {} and pre-computed vertices", size);
    glCreateBuffers(1, &m_id);
    glNamedBufferData(m_id, size, vertices, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
    spdlog::debug("Deleting VertexBuffer instance");
}

void VertexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void VertexBuffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

const std::uint32_t& VertexBuffer::getID() const
{
    return m_id;
}

const BufferLayout& VertexBuffer::getLayout() const
{
    return m_layout;
}

void VertexBuffer::setData(const void* data, const std::uint32_t& size)
{
    spdlog::debug("Setting drawing data to VertexBuffer");
    glNamedBufferSubData(m_id, 0, size, data);
}

void VertexBuffer::setLayout(const BufferLayout& layout)
{
    m_layout = layout;
}

VertexBuffer::operator std::uint32_t() const
{
    return m_id;
}

///////////////////////////////////////////////////////////////////////////////

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
    : m_name(name),
      m_shaderType(type),
      m_size(sizeofShaderDataType(type)),
      m_offset(0),
      m_normalized(normalized)
{
}

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
