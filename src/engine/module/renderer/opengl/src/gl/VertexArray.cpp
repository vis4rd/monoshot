#include "../../include/gl/VertexArray.hpp"

#include <stdexcept>

#include <glad/gl.h>
#include <spdlog/spdlog.h>

#include "../../include/gl/ShaderDataType.hpp"

namespace mono::gl
{

static GLenum shaderDataTypeToOpenGLBaseType(const ShaderDataType& type)
{
    switch(type)
    {
        case ShaderDataType::FLOAT1: return GL_FLOAT;
        case ShaderDataType::FLOAT2: return GL_FLOAT;
        case ShaderDataType::FLOAT3: return GL_FLOAT;
        case ShaderDataType::FLOAT4: return GL_FLOAT;
        case ShaderDataType::MAT3: return GL_FLOAT;
        case ShaderDataType::MAT4: return GL_FLOAT;
        case ShaderDataType::INT1: return GL_INT;
        case ShaderDataType::INT2: return GL_INT;
        case ShaderDataType::INT3: return GL_INT;
        case ShaderDataType::INT4: return GL_INT;
        case ShaderDataType::BOOL1: return GL_BOOL;
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

VertexArray::VertexArray(VertexArray&& move) noexcept
    : m_id(move.m_id)
    , m_attributeBindingCount(move.m_attributeBindingCount)
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

VertexArray& VertexArray::operator=(VertexArray&& move) noexcept
{
    m_id = move.m_id;
    m_attributeBindingCount = move.m_attributeBindingCount;
    m_vertexBuffers = std::move(move.m_vertexBuffers);
    m_elementBuffer = std::move(move.m_elementBuffer);
    return *this;
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
    spdlog::debug(
        "Adding a VertexBuffer with ID = {} to VertexArray with ID = {}",
        vertex_buffer.getID(),
        m_id);
    if(vertex_buffer.getLayout().getAttributes().empty())
    {
        spdlog::error("Given VertexBuffer does not have a specified layout");
        throw std::runtime_error("Given VertexBuffer does not have a specified layout");
    }

    std::uint32_t sum_size = 0;
    for(const auto& element : vertex_buffer.getLayout())
    {
        sum_size += element.getSize();
    }
    glVertexArrayVertexBuffer(
        m_id,
        /*vbo index for this vao*/ m_vertexBuffers.size(),
        /*vbo id*/ vertex_buffer,
        /*offset*/ 0,
        static_cast<std::int32_t>(sum_size));

    const auto& layout = vertex_buffer.getLayout();
    for(const auto& element : layout)
    {
        auto& type = element.getShaderDataType();
        switch(type)
        {
            case ShaderDataType::FLOAT1:
            case ShaderDataType::FLOAT2:
            case ShaderDataType::FLOAT3:
            case ShaderDataType::FLOAT4:
            {
                spdlog::debug("VertexArray: VertexBuffer element's type is a float");
                glVertexArrayAttribFormat(
                    m_id,
                    m_attributeBindingCount,
                    static_cast<std::int32_t>(element.getComponentCount()),
                    shaderDataTypeToOpenGLBaseType(element.getShaderDataType()),
                    element.isNormalized(),
                    element.getOffset());
                glVertexArrayAttribBinding(m_id, m_attributeBindingCount, m_vertexBuffers.size());
                // glBindVertexArray(m_id);
                // glVertexAttribDivisor(
                //     m_attributeBindingCount,
                //     static_cast<GLuint>(element.getUpdateFrequency()));
                // glBindVertexArray(0);
                //! glVertexArrayBindingDivisor sets step size for whole vbo binding, not the single
                //! attribute
                glVertexArrayBindingDivisor(
                    m_id,
                    m_vertexBuffers.size(),
                    static_cast<GLuint>(element.getUpdateFrequency()));
                glEnableVertexArrayAttrib(m_id, m_attributeBindingCount);
                m_attributeBindingCount++;
                break;
            }
            case ShaderDataType::INT1:
            case ShaderDataType::INT2:
            case ShaderDataType::INT3:
            case ShaderDataType::INT4:
            case ShaderDataType::BOOL1:
            {
                spdlog::debug("VertexArray: VertexBuffer element's type is an integer");
                glVertexArrayAttribIFormat(
                    m_id,
                    m_attributeBindingCount,
                    static_cast<std::int32_t>(element.getComponentCount()),
                    shaderDataTypeToOpenGLBaseType(element.getShaderDataType()),
                    element.getOffset());
                glVertexArrayAttribBinding(m_id, m_attributeBindingCount, m_vertexBuffers.size());
                glBindVertexArray(m_id);
                // glVertexAttribDivisor(
                //     m_attributeBindingCount,
                //     static_cast<GLuint>(element.getUpdateFrequency()));
                // glBindVertexArray(0);
                //! glVertexArrayBindingDivisor sets step size for whole vbo binding, not the single
                //! attribute
                glVertexArrayBindingDivisor(
                    m_id,
                    m_vertexBuffers.size(),
                    static_cast<GLuint>(element.getUpdateFrequency()));
                glEnableVertexArrayAttrib(m_id, m_attributeBindingCount);
                m_attributeBindingCount++;
                break;
            }
            case ShaderDataType::MAT3:
            case ShaderDataType::MAT4:
            {
                spdlog::debug("VertexArray: VertexBuffer element's type is a matrix");
                const std::uint8_t count = element.getComponentCount();
                for(std::uint8_t i = 0; i < count; i++)
                {
                    glVertexArrayAttribFormat(
                        m_id,
                        m_attributeBindingCount,
                        static_cast<GLint>(element.getComponentCount()),
                        shaderDataTypeToOpenGLBaseType(element.getShaderDataType()),
                        element.isNormalized(),
                        (element.getOffset() + sizeof(float) * count * i));

                    glVertexArrayAttribBinding(
                        m_id,
                        m_attributeBindingCount,
                        m_vertexBuffers.size());
                    // glBindVertexArray(m_id);
                    // glVertexAttribDivisor(
                    //     m_attributeBindingCount,
                    //     static_cast<GLuint>(element.getUpdateFrequency()));
                    // glBindVertexArray(0);
                    //! glVertexArrayBindingDivisor sets step size for whole vbo binding, not the
                    //! single attribute
                    glVertexArrayBindingDivisor(
                        m_id,
                        m_vertexBuffers.size(),
                        static_cast<GLuint>(element.getUpdateFrequency()));
                    glEnableVertexArrayAttrib(m_id, m_attributeBindingCount);
                    m_attributeBindingCount++;
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
    spdlog::debug(
        "Adding ElementBuffer with ID = {} to VertexArray with ID = {}",
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

}  // namespace mono::gl
