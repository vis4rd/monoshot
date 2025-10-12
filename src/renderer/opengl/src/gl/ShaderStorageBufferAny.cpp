#include "../../include/opengl/gl/ShaderStorageBufferAny.hpp"

namespace mono::gl
{
ShaderStorageBufferAny::ShaderStorageBufferAny(::gl::GLsizeiptr size)
    : m_maxBufferBytesize{size}
{
    ::gl::glCreateBuffers(1, &m_id);
    ::gl::glNamedBufferStorage(m_id, m_maxBufferBytesize, nullptr, ::gl::GL_DYNAMIC_STORAGE_BIT);
    log::setGlObjectLabel(::gl::GL_BUFFER, m_id, "ShaderStorageBufferAny#{}", m_id);
    spdlog::debug(
        "Created ShaderStorageBuffer instance with ID = {} and size = {}",
        m_id,
        m_maxBufferBytesize);
}

void ShaderStorageBufferAny::bind(::gl::GLuint binding) const
{
    // spdlog::trace("Binding ShaderStorageBuffer with ID = {}", m_id);
    ::gl::glBindBufferBase(::gl::GL_SHADER_STORAGE_BUFFER, binding, m_id);
}

void ShaderStorageBufferAny::unbind() const
{
    // spdlog::trace("Unbinding ShaderStorageBuffer with ID = {}", m_id);
    ::gl::glBindBuffer(::gl::GL_SHADER_STORAGE_BUFFER, 0);
}

const ::gl::GLuint& ShaderStorageBufferAny::getID() const
{
    return m_id;
}

void ShaderStorageBufferAny::resize(::gl::GLsizeiptr new_byte_size)
{
    spdlog::debug("Resizing ShaderStorageBuffer with ID = {} to {} bytes", m_id, new_byte_size);
    // create a new ssbo with the new size
    ::gl::GLuint new_id{};
    ::gl::glCreateBuffers(1, &new_id);
    ::gl::glNamedBufferStorage(new_id, new_byte_size, nullptr, ::gl::GL_DYNAMIC_STORAGE_BIT);

    // copy data from old ssbo to the new one
    ::gl::glCopyNamedBufferSubData(
        m_id,
        new_id,
        0,
        0,
        std::min(m_maxBufferBytesize, new_byte_size));

    // unbind and delete the old ssbo
    ::gl::glBindBuffer(::gl::GL_SHADER_STORAGE_BUFFER, 0);
    ::gl::glDeleteBuffers(1, &m_id);

    // track the new ssbo
    m_id = new_id;
    m_maxBufferBytesize = new_byte_size;
}

ShaderStorageBufferAny::operator ::gl::GLuint() const
{
    return m_id;
}
}  // namespace mono::gl
