#pragma once

#include <cstdint>
#include <string>
#include <vector>

class BufferElement;  // pre-declaration

class BufferLayout
{
    public:
    BufferLayout() = default;
    BufferLayout(std::initializer_list<BufferElement> elements);

    std::uint32_t getStride() const;
    const std::vector<BufferElement>& getElements() const;

    std::vector<BufferElement>::iterator begin();
    std::vector<BufferElement>::iterator end();
    std::vector<BufferElement>::const_iterator begin() const;
    std::vector<BufferElement>::const_iterator end() const;

    private:
    void calculateOffsetAndStride();

    private:
    std::vector<BufferElement> m_elements;
    std::uint32_t m_stride = 0u;
};

///////////////////////////////////////////////////////////////////////////////

class VertexBuffer
{
    public:
    VertexBuffer(std::uint32_t size);
    VertexBuffer(float* vertices, std::uint32_t size);
    VertexBuffer(const VertexBuffer& copy);
    VertexBuffer(VertexBuffer&& move);
    ~VertexBuffer();

    void bind() const;
    void unbind() const;

    const BufferLayout& getLayout() const;

    void setData(const void* data, const std::uint32_t& size);
    void setLayout(const BufferLayout& layout);

    operator std::uint32_t() const;

    private:
    std::uint32_t m_id;
    BufferLayout m_layout;
};

///////////////////////////////////////////////////////////////////////////////

enum class ShaderDataType
{
    none = 0,
    float1,
    float2,
    float3,
    float4,
    mat3,
    mat4,
    int1,
    int2,
    int3,
    int4,
    bool1
};

class BufferElement
{
    public:
    BufferElement() = default;
    BufferElement(const ShaderDataType& type, const std::string& name, bool normalized = false);

    const std::string& getName() const;
    const ShaderDataType& getShaderDataType() const;
    std::uint32_t getSize() const;
    std::size_t getOffset() const;
    bool isNormalized() const;
    std::uint32_t getComponentCount() const;

    void setSize(const std::uint32_t& size);
    void setOffset(const std::size_t& offset);

    private:
    std::string m_name;
    ShaderDataType m_shaderType;
    std::uint32_t m_size;
    std::size_t m_offset;
    bool m_normalized;
};
