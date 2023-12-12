#pragma once

#include <cstdint>
#include <string>

#include "ShaderAttributeUpdateFrequency.hpp"
#include "ShaderDataType.hpp"

namespace mono::gl
{

class ShaderAttribute
{
    public:
    ShaderAttribute() = default;
    ShaderAttribute(ShaderDataType type, const std::string& name);
    ShaderAttribute(
        ShaderDataType type,
        const std::string& name,
        bool normalized,
        ShaderAttributeUpdateFrequency frequency = ShaderAttributeUpdateFrequency::EACH_VERTEX,
        std::size_t offset = 0llu);
    ShaderAttribute(
        ShaderDataType type,
        const std::string& name,
        ShaderAttributeUpdateFrequency frequency,
        std::size_t offset = 0llu,
        bool normalized = false);
    ShaderAttribute(
        ShaderDataType type,
        const std::string& name,
        std::size_t offset,
        bool normalized = false,
        ShaderAttributeUpdateFrequency frequency = ShaderAttributeUpdateFrequency::EACH_VERTEX);


    const std::string& getName() const;
    ShaderDataType getShaderDataType() const;
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
    std::size_t m_offset{0llu};
    bool m_normalized{false};
    ShaderAttributeUpdateFrequency m_frequency{ShaderAttributeUpdateFrequency::EACH_VERTEX};
};

}  // namespace mono::gl
