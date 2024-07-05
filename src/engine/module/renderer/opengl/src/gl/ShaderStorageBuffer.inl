namespace mono::gl
{

template<typename T>
constexpr ShaderStorageBuffer<T>::ShaderStorageBuffer(GLsizeiptr size)
    : m_maxBufferBytesize(size)
{
    glCreateBuffers(1, &m_id);
    glNamedBufferStorage(m_id, m_maxBufferBytesize, nullptr, GL_DYNAMIC_STORAGE_BIT);
    spdlog::debug(
        "Created ShaderStorageBuffer instance with ID = {} and size = {}",
        m_id,
        m_maxBufferBytesize);
}

template<typename T>
void ShaderStorageBuffer<T>::bind(GLuint binding) const
{
    spdlog::trace("Binding ShaderStorageBuffer with ID = {}", m_id);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, m_id);
}

template<typename T>
void ShaderStorageBuffer<T>::unbind() const
{
    spdlog::trace("Unbinding ShaderStorageBuffer with ID = {}", m_id);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

template<typename T>
const GLuint& ShaderStorageBuffer<T>::getID() const
{
    return m_id;
}

template<typename T>
constexpr void ShaderStorageBuffer<T>::setData(
    const ContiguousContainerTrait<T> auto& data,
    GLintptr buffer_offset)
{
    auto size = static_cast<GLsizeiptr>(data.size() * sizeof(value_type));
    spdlog::trace("Setting drawing data to ShaderStorageBuffer with ID = {}", m_id);

    if((size + buffer_offset) > m_maxBufferBytesize)
    {
        spdlog::warn(
            "Data size ({} bytes) with offset ({} bytes) reaches outside of buffer size ({} bytes)",
            size,
            buffer_offset,
            m_maxBufferBytesize);
        this->resize(size + buffer_offset);
    }

    glNamedBufferSubData(m_id, buffer_offset, size, data.data());
}

template<typename T>
void ShaderStorageBuffer<T>::resize(GLsizeiptr new_byte_size)
{
    spdlog::debug("Resizing ShaderStorageBuffer with ID = {} to {} bytes", m_id, new_byte_size);
    // create a new ssbo with the new size
    GLuint new_id{};
    glCreateBuffers(1, &new_id);
    glNamedBufferStorage(new_id, new_byte_size, nullptr, GL_DYNAMIC_STORAGE_BIT);

    // copy data from old ssbo to the new one
    glCopyNamedBufferSubData(m_id, new_id, 0, 0, std::min(m_maxBufferBytesize, new_byte_size));

    // unbind and delete the old ssbo
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    glDeleteBuffers(1, &m_id);

    // track the new ssbo
    m_id = new_id;
    m_maxBufferBytesize = new_byte_size;
}

template<typename T>
ShaderStorageBuffer<T>::operator GLuint() const
{
    return m_id;
}

}  // namespace mono::gl
