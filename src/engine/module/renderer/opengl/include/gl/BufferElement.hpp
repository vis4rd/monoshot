#pragma once

#include <cstdint>
#include <string>

#include "ShaderDataType.hpp"

namespace mono
{

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

}  // namespace mono
