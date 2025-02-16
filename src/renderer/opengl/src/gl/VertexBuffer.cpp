#include "../../include/opengl/gl/VertexBuffer.hpp"

namespace mono::gl
{

VertexBuffer::VertexBuffer(GLsizeiptr size)
    : m_maxBufferBytesize(size)
{
    glCreateBuffers(1, &m_id);
    glNamedBufferData(m_id, m_maxBufferBytesize, nullptr, GL_DYNAMIC_DRAW);
    log::setGlObjectLabel(GL_BUFFER, m_id, "VertexBuffer#{}", m_id);
    spdlog::debug(
        "Created VertexBuffer instance with ID = {} and size = {}",
        m_id,
        m_maxBufferBytesize);
}

VertexBuffer::VertexBuffer(const VertexBuffer& copy)
    : m_id(copy.m_id)
    , m_layout(copy.m_layout)
    , m_maxBufferBytesize(copy.m_maxBufferBytesize)
{
    spdlog::debug("Copying VertexBuffer instance with ID = {}", copy.m_id);
}

VertexBuffer::VertexBuffer(VertexBuffer&& move) noexcept
    : m_id(move.m_id)
    , m_layout(std::move(move.m_layout))
    , m_maxBufferBytesize(move.m_maxBufferBytesize)

{
    spdlog::debug("Moving VertexBuffer instance with ID = {}", m_id);
}

VertexBuffer::VertexBuffer(const float* vertices, GLsizeiptr size)
    : m_maxBufferBytesize(size)
{
    spdlog::debug("Creating VertexBuffer...");
    glCreateBuffers(1, &m_id);
    glNamedBufferData(m_id, m_maxBufferBytesize, vertices, GL_STATIC_DRAW);
    spdlog::debug(
        "Created VertexBuffer instance with ID = {}, size = {} and pre-computed vertices",
        m_id,
        m_maxBufferBytesize);
}

VertexBuffer::~VertexBuffer()
{
    spdlog::debug("Calling destructor of VertexBuffer with ID = {} (no destroy)", m_id);
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& move) noexcept
{
    m_id = move.m_id;
    m_layout = std::move(move.m_layout);
    return *this;
}

void VertexBuffer::bind() const
{
    // spdlog::trace("Binding VertexBuffer with ID = {}", m_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void VertexBuffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

const GLuint& VertexBuffer::getID() const
{
    return m_id;
}

ShaderAttributeLayout& VertexBuffer::getLayout()
{
    return m_layout;
}

void VertexBuffer::setData(const void* data, GLsizeiptr size)
{
    if(size > m_maxBufferBytesize)
    {
        spdlog::warn(
            "Data size ({} bytes) is bigger than buffer size ({} bytes). Resizing buffer...",
            size,
            m_maxBufferBytesize);
        this->resize(size);
    }

    glNamedBufferSubData(m_id, 0, size, data);
}

void VertexBuffer::setLayout(const ShaderAttributeLayout& layout)
{
    m_layout = layout;
}

VertexBuffer::operator GLuint() const
{
    return m_id;
}

void VertexBuffer::resize(GLsizeiptr new_byte_size)
{
    // create a new vbo with the new size
    GLuint new_id{};
    glCreateBuffers(1, &new_id);
    glNamedBufferData(m_id, new_byte_size, nullptr, GL_DYNAMIC_DRAW);

    // unbind and delete the old vbo
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &m_id);

    // track the new vbo
    m_id = new_id;
    m_maxBufferBytesize = new_byte_size;
}

}  // namespace mono::gl
