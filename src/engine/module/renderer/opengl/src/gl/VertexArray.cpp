#include "../../include/gl/VertexArray.hpp"

#include <stdexcept>

#include <glad/gl.h>
#include <spdlog/spdlog.h>

#include "../../include/gl/ShaderDataType.hpp"

namespace mono
{

static GLenum shaderDataTypeToOpenGLBaseType(const ShaderDataType& type)
{
    switch(type)
    {
        case ShaderDataType::float1: return GL_FLOAT;
        case ShaderDataType::float2: return GL_FLOAT;
        case ShaderDataType::float3: return GL_FLOAT;
        case ShaderDataType::float4: return GL_FLOAT;
        case ShaderDataType::mat3: return GL_FLOAT;
        case ShaderDataType::mat4: return GL_FLOAT;
        case ShaderDataType::int1: return GL_INT;
        case ShaderDataType::int2: return GL_INT;
        case ShaderDataType::int3: return GL_INT;
        case ShaderDataType::int4: return GL_INT;
        case ShaderDataType::bool1: return GL_BOOL;
        default:
        {
            spdlog::error("Unknown ShaderDataType, returning 0");
            return 0;
        }
    }
}

VertexArray::VertexArray()
{
    glCreateVertexArrays(1, &m_id);
    spdlog::debug("Created VertexArray instance with ID = {}", m_id);
}

VertexArray::VertexArray(const VertexArray& copy)
    : m_id(copy.m_id)
    , m_vertexBufferIndex(copy.m_vertexBufferIndex)
    , m_vertexBuffers(copy.m_vertexBuffers)
    , m_elementBuffer(copy.m_elementBuffer)
{ }

VertexArray::VertexArray(VertexArray&& move)
    : m_id(std::move(move.m_id))
    , m_vertexBufferIndex(std::move(move.m_vertexBufferIndex))
    , m_vertexBuffers(std::move(move.m_vertexBuffers))
    , m_elementBuffer(std::move(move.m_elementBuffer))
{ }

VertexArray::~VertexArray()
{
    spdlog::debug("Deleting VertexArray instance with ID = {}", m_id);
    for(const auto& vb : m_vertexBuffers)
    {
        spdlog::debug("Deleting VertexBuffer object with ID = {}", vb.getID());
        glDeleteBuffers(1, &vb.getID());
    }
    m_vertexBuffers.clear();
    if(m_elementBuffer.isInitialized())
    {
        spdlog::debug("Deleting ElementBuffer object with ID = {}", m_elementBuffer.getID());
        glDeleteBuffers(1, &m_elementBuffer.getID());
    }
    glDeleteVertexArrays(1, &m_id);
    this->unbind();
}

void VertexArray::bind() const
{
    spdlog::trace("Binding VertexArray with ID = {}", m_id);
    glBindVertexArray(m_id);
}

void VertexArray::unbind() const
{
    glBindVertexArray(0);
}

void VertexArray::addVertexBuffer(VertexBuffer&& vertex_buffer)
{
    spdlog::debug("Adding a VertexBuffer with ID = {} to VertexArray with ID = {}",
        vertex_buffer.getID(),
        m_id);
    if(vertex_buffer.getLayout().getElements().empty())
    {
        spdlog::error("Given VertexBuffer does not have a specified layout");
        throw std::runtime_error("Given VertexBuffer does not have a specified layout");
    }

    std::uint32_t sum_size = 0;
    for(const auto& element : vertex_buffer.getLayout())
    {
        sum_size += element.getSize();
    }
    glVertexArrayVertexBuffer(m_id,
        /*vbo index for this vao*/ m_vertexBuffers.size(),
        /*vbo id*/ vertex_buffer,
        /*offset*/ 0,
        sum_size);

    const auto& layout = vertex_buffer.getLayout();
    for(const auto& element : layout)
    {
        auto& type = element.getShaderDataType();
        switch(type)
        {
            case ShaderDataType::float1:
            case ShaderDataType::float2:
            case ShaderDataType::float3:
            case ShaderDataType::float4:
            {
                spdlog::debug("VertexArray: VertexBuffer element's type is a float");
                glVertexArrayAttribFormat(m_id,
                    m_vertexBufferIndex,
                    element.getComponentCount(),
                    shaderDataTypeToOpenGLBaseType(element.getShaderDataType()),
                    element.isNormalized(),
                    element.getOffset());
                glVertexArrayAttribBinding(m_id, m_vertexBufferIndex, m_vertexBuffers.size());
                glEnableVertexArrayAttrib(m_id, m_vertexBufferIndex);
                m_vertexBufferIndex++;
                break;
            }
            case ShaderDataType::int1:
            case ShaderDataType::int2:
            case ShaderDataType::int3:
            case ShaderDataType::int4:
            case ShaderDataType::bool1:
            {
                spdlog::debug("VertexArray: VertexBuffer element's type is an integer");
                glVertexArrayAttribIFormat(m_id,
                    m_vertexBufferIndex,
                    element.getComponentCount(),
                    shaderDataTypeToOpenGLBaseType(element.getShaderDataType()),
                    element.getOffset());
                glVertexArrayAttribBinding(m_id, m_vertexBufferIndex, m_vertexBuffers.size());
                glEnableVertexArrayAttrib(m_id, m_vertexBufferIndex);
                m_vertexBufferIndex++;
                break;
            }
            case ShaderDataType::mat3:
            case ShaderDataType::mat4:
            {
                spdlog::debug("VertexArray: VertexBuffer element's type is a matrix");
                std::uint8_t count = element.getComponentCount();
                for(std::uint8_t i = 0; i < count; i++)
                {
                    glVertexArrayAttribFormat(m_id,
                        m_vertexBufferIndex,
                        element.getComponentCount(),
                        shaderDataTypeToOpenGLBaseType(element.getShaderDataType()),
                        element.isNormalized(),
                        (element.getOffset() + sizeof(float) * count * i));
                    glVertexArrayAttribBinding(m_id, m_vertexBufferIndex, m_vertexBuffers.size());
                    glEnableVertexArrayAttrib(m_id, m_vertexBufferIndex);
                    glVertexAttribDivisor(m_vertexBufferIndex, 1);
                    m_vertexBufferIndex++;
                }
                break;
            }
            default:
            {
                spdlog::error("Unknown ShaderDataType");
                break;
            }
        }
    }
    m_vertexBuffers.push_back(std::move(vertex_buffer));
}

void VertexArray::addElementBuffer(const ElementBuffer& element_buffer)
{
    spdlog::debug("Adding ElementBuffer with ID = {} to VertexArray with ID = {}",
        element_buffer.getID(),
        m_id);
    if(!element_buffer.isInitialized())
    {
        spdlog::error(
            "Given ElementBuffer is not initialized! Pass data to it on construction or through setData() method");
    }
    glVertexArrayElementBuffer(m_id, element_buffer);

    m_elementBuffer = element_buffer;
}

std::vector<VertexBuffer>& VertexArray::getVertexBuffers()
{
    return m_vertexBuffers;
}

const std::vector<VertexBuffer>& VertexArray::getVertexBuffers() const
{
    return m_vertexBuffers;
}

const ElementBuffer& VertexArray::getElementBuffer() const
{
    return m_elementBuffer;
}

VertexArray::operator std::uint32_t()
{
    return m_id;
}

}  // namespace mono
