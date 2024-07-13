namespace mono::gl
{

constexpr void ShaderStorageBufferAny::setData(const auto& data, GLintptr buffer_offset)
requires(
    not std::ranges::range<decltype(data)> and not std::is_pointer_v<decltype(data)>
    and not std::is_array_v<decltype(data)>)
{
    using value_type = std::remove_cvref_t<decltype(data)>;

    GLsizeiptr size = sizeof(value_type);
    // spdlog::trace("Setting drawing data to ShaderStorageBufferAny with ID = {}", m_id);

    if((size + buffer_offset) > m_maxBufferBytesize)
    {
        spdlog::debug(
            "Data size ({} bytes) with offset ({} bytes) reaches outside of buffer size ({} bytes)",
            size,
            buffer_offset,
            m_maxBufferBytesize);
        this->resize(size + buffer_offset);
    }

    glNamedBufferSubData(m_id, buffer_offset, size, &data);
}

constexpr void ShaderStorageBufferAny::setData(
    const std::ranges::contiguous_range auto& data,
    GLintptr buffer_offset)
{
    using value_type = std::remove_cvref_t<decltype(data)>::value_type;

    auto size = static_cast<GLsizeiptr>(data.size() * sizeof(value_type));
    // spdlog::trace("Setting drawing data to ShaderStorageBufferAny with ID = {}", m_id);

    if((size + buffer_offset) > m_maxBufferBytesize)
    {
        spdlog::debug(
            "Data size ({} bytes) with offset ({} bytes) reaches outside of buffer size ({} bytes)",
            size,
            buffer_offset,
            m_maxBufferBytesize);
        this->resize(size + buffer_offset);
    }

    glNamedBufferSubData(m_id, buffer_offset, size, data.data());
}

}  // namespace mono::gl
