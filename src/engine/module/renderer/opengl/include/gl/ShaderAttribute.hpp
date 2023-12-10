#pragma once

#include <cstdint>
#include <string>

#include "ShaderAttributeUpdateFrequency.hpp"
#include "ShaderDataType.hpp"

namespace mono
{

class ShaderAttribute
{
    public:
    ShaderAttribute() = default;
    ShaderAttribute(
        const ShaderDataType& type,
        const std::string& name,
        bool normalized = false,
        ShaderAttributeUpdateFrequency frequency = ShaderAttributeUpdateFrequency::EACH_VERTEX,
        std::size_t offset = 0llu);

    const std::string& getName() const;
    const ShaderDataType& getShaderDataType() const;
    std::uint32_t getSize() const;
    std::size_t getOffset() const;
    bool isNormalized() const;
    std::uint32_t getComponentCount() const;
    ShaderAttributeUpdateFrequency getUpdateFrequency() const;

    void setSize(const std::uint32_t& size);
    void setOffset(const std::size_t& offset);
    void setUpdateFrequency(ShaderAttributeUpdateFrequency frequency);

    private:
    std::string m_name{};
    ShaderDataType m_shaderType{};
    std::uint32_t m_size{};
    std::size_t m_offset{};
    bool m_normalized{};
    ShaderAttributeUpdateFrequency m_frequency{ShaderAttributeUpdateFrequency::EACH_VERTEX};
};

}  // namespace mono
