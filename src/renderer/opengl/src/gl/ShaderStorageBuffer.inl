namespace mono::gl
{

template<typename T>
constexpr ShaderStorageBuffer<T>::ShaderStorageBuffer(GLsizeiptr size)
    : ShaderStorageBufferAny{size}
{ }

template<typename T>
constexpr void ShaderStorageBuffer<T>::setData(
    const ContiguousContainerTrait<T> auto& data,
    GLintptr buffer_offset)
{
    ShaderStorageBufferAny::setData(data, buffer_offset);
}

}  // namespace mono::gl
