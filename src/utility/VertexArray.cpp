#include "../../include/utility/VertexArray.hpp"
#include <glad/glad.h>
#include <stdexcept>
#include <spdlog/spdlog.h>

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
}

VertexArray::~VertexArray()
{
    this->unbind();
    glDeleteVertexArrays(1, &m_id);
}

void VertexArray::bind() const
{
    glBindVertexArray(m_id);
}

void VertexArray::unbind() const
{
    glBindVertexArray(0);
}

void VertexArray::addVertexBuffer(const VertexBuffer& vertex_buffer)
{
    if(vertex_buffer.getLayout().getElements().empty())
    {
        spdlog::error("Given VertexBuffer does not have a specified layout");
        throw std::runtime_error("Given VertexBuffer does not have a specified layout");
    }

    // this->bind();
    // vertex_buffer.bind();
    std::uint32_t sum_size = 0;
    for(const auto& element : vertex_buffer.getLayout())
    {
        sum_size += element.getSize();
    }
    glVertexArrayVertexBuffer(m_id, /*vbo index for this vao*/ m_vertexBuffers.size(), /*vbo id*/ vertex_buffer, /*offset*/ 0, sum_size);

    // // glVertexArrayAttribFormat(s_data.quadVao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(QuadVertex, position));
    // // glVertexArrayAttribBinding(s_data.quadVao, 0, /*vbo index for this vao*/ 0);
    // // glEnableVertexArrayAttrib(s_data.quadVao, 0);

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
                glVertexArrayAttribFormat(m_id, m_vertexBufferIndex, element.getSize(), shaderDataTypeToOpenGLBaseType(element.getShaderDataType()), element.isNormalized(), element.getOffset());
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
                glVertexArrayAttribIFormat(m_id, m_vertexBufferIndex, element.getSize(), shaderDataTypeToOpenGLBaseType(element.getShaderDataType()), element.getOffset());
                glVertexArrayAttribBinding(m_id, m_vertexBufferIndex, m_vertexBuffers.size());
                glEnableVertexArrayAttrib(m_id, m_vertexBufferIndex);
                m_vertexBufferIndex++;
                break;
            }
            case ShaderDataType::mat3:
            case ShaderDataType::mat4:
            {
                std::uint8_t count = element.getComponentCount();
                for(std::uint8_t i = 0; i < count; i++)
                {
                    glVertexArrayAttribFormat(m_id, m_vertexBufferIndex, element.getSize(), shaderDataTypeToOpenGLBaseType(element.getShaderDataType()), element.isNormalized(), (element.getOffset() + sizeof(float) * count * i));
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

    // const auto& layout = vertex_buffer.getLayout();
    // for(const auto& element : layout)
    // {
    //     switch(element.getShaderDataType())
    //     {
    //         case ShaderDataType::float1:
    //         case ShaderDataType::float2:
    //         case ShaderDataType::float3:
    //         case ShaderDataType::float4:
    //         {
    //             glEnableVertexAttribArray(m_vertexBufferIndex);
    //             glVertexAttribPointer(m_vertexBufferIndex,
    //                 element.getComponentCount(),
    //                 shaderDataTypeToOpenGLBaseType(element.getShaderDataType()),
    //                 element.isNormalized() ? GL_TRUE : GL_FALSE,
    //                 layout.getStride(),
    //                 reinterpret_cast<const void*>(element.getOffset()));
    //             m_vertexBufferIndex++;
    //             break;
    //         }
    //         case ShaderDataType::int1:
    //         case ShaderDataType::int2:
    //         case ShaderDataType::int3:
    //         case ShaderDataType::int4:
    //         case ShaderDataType::bool1:
    //         {
    //             glEnableVertexAttribArray(m_vertexBufferIndex);
    //             glVertexAttribIPointer(m_vertexBufferIndex, element.getComponentCount(), shaderDataTypeToOpenGLBaseType(element.getShaderDataType()), layout.getStride(), reinterpret_cast<const void*>(element.getOffset()));
    //             m_vertexBufferIndex++;
    //             break;
    //         }
    //         case ShaderDataType::mat3:
    //         case ShaderDataType::mat4:
    //         {
    //             std::uint8_t count = element.getComponentCount();
    //             for(std::uint8_t i = 0; i < count; i++)
    //             {
    //                 glEnableVertexAttribArray(m_vertexBufferIndex);
    //                 glVertexAttribPointer(m_vertexBufferIndex,
    //                     count,
    //                     shaderDataTypeToOpenGLBaseType(element.getShaderDataType()),
    //                     element.isNormalized() ? GL_TRUE : GL_FALSE,
    //                     layout.getStride(),
    //                     reinterpret_cast<const void*>(element.getOffset() + sizeof(float) * count * i));
    //                 glVertexAttribDivisor(m_vertexBufferIndex, 1);
    //                 m_vertexBufferIndex++;
    //             }
    //             break;
    //         }
    //         default:
    //         {
    //             spdlog::error("Unknown ShaderDataType");
    //             break;
    //         }
    //     }
    // }
    m_vertexBuffers.push_back(vertex_buffer);
}

void VertexArray::addElementBuffer(const ElementBuffer& element_buffer)
{
    // glBindVertexArray(m_id);
    // element_buffer.bind();
    glVertexArrayElementBuffer(m_id, element_buffer);

    m_elementBuffer = std::make_unique<ElementBuffer>(element_buffer);
}

const std::vector<VertexBuffer>& VertexArray::getVertexBuffers() const
{
    return m_vertexBuffers;
}

const std::unique_ptr<ElementBuffer>& VertexArray::getElementBuffer() const
{
    return m_elementBuffer;
}

VertexArray::operator std::uint32_t()
{
    return m_id;
}
