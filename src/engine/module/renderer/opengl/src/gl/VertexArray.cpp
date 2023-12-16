#include "../../include/gl/VertexArray.hpp"

#include <stdexcept>

#include <glad/gl.h>
#include <spdlog/spdlog.h>

#include "../../include/gl/ShaderAttributeType.hpp"

namespace mono::gl
{

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
        sum_size += element.getBytesize();
    }
    glVertexArrayVertexBuffer(
        m_id,
        /*vbo index for this vao*/ m_vertexBuffers.size(),
        /*vbo id*/ vertex_buffer,
        /*offset*/ 0,
        static_cast<std::int32_t>(sum_size));

    const auto& layout = vertex_buffer.getLayout();
    for(const auto& attribute : layout)
    {
        //* set after attrib binding
        // glBindVertexArray(m_id);
        // glVertexAttribDivisor(
        //     m_attributeBindingCount,
        //     static_cast<GLuint>(element.getUpdateFrequency()));
        // glBindVertexArray(0);
        //*
        // new here
        auto type = attribute.getType();
        const auto count = attribute.getComponentCount();
        for(std::uint32_t i = 0; i < count; i++)
        {
            switch(type.glType)
            {
                case GL_BOOL:
                case GL_INT:
                case GL_INT_2_10_10_10_REV:
                case GL_UNSIGNED_INT:
                case GL_UNSIGNED_INT_2_10_10_10_REV:
                {
                    glVertexArrayAttribIFormat(
                        m_id,
                        m_attributeBindingCount,
                        type.valuesPerVertex,
                        type.glType,
                        attribute.getOffset() + type.sizeofNativeType * count * i);
                    break;
                }
                case GL_DOUBLE:
                {
                    glVertexArrayAttribLFormat(
                        m_id,
                        m_attributeBindingCount,
                        type.valuesPerVertex,
                        type.glType,
                        attribute.getOffset() + type.sizeofNativeType * count * i);
                    break;
                }
                case GL_BYTE:
                case GL_FIXED:
                case GL_FLOAT:
                case GL_HALF_FLOAT:
                case GL_SHORT:
                case GL_UNSIGNED_BYTE:
                case GL_UNSIGNED_INT_10F_11F_11F_REV:
                case GL_UNSIGNED_SHORT:
                default:
                {
                    glVertexArrayAttribFormat(
                        m_id,
                        m_attributeBindingCount,
                        type.valuesPerVertex,
                        type.glType,
                        attribute.isNormalized(),
                        attribute.getOffset() + type.sizeofNativeType * count * i);
                    break;
                }
            }
            glVertexArrayAttribBinding(m_id, m_attributeBindingCount, m_vertexBuffers.size());
            glVertexArrayBindingDivisor(
                m_id,
                m_vertexBuffers.size(),
                static_cast<GLuint>(attribute.getUpdateFrequency()));
            glEnableVertexArrayAttrib(m_id, m_attributeBindingCount);
            m_attributeBindingCount++;
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
