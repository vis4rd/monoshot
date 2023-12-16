#pragma once

#include <cstdint>
#include <string>

#include "ShaderAttributeType.hpp"
#include "ShaderAttributeUpdateFrequency.hpp"

namespace mono::gl
{

class ShaderAttribute
{
    public:
    ShaderAttribute(const ShaderAttributeTypeInfo& type, const std::string& name);
    ShaderAttribute(
        const ShaderAttributeTypeInfo& type,
        const std::string& name,
        bool normalized,
        ShaderAttributeUpdateFrequency frequency = ShaderAttributeUpdateFrequency::EACH_VERTEX,
        std::size_t offset = 0llu);
    ShaderAttribute(
        const ShaderAttributeTypeInfo& type,
        const std::string& name,
        ShaderAttributeUpdateFrequency frequency,
        std::size_t offset = 0llu,
        bool normalized = false);
    ShaderAttribute(
        const ShaderAttributeTypeInfo& type,
        const std::string& name,
        std::size_t offset,
        bool normalized = false,
        ShaderAttributeUpdateFrequency frequency = ShaderAttributeUpdateFrequency::EACH_VERTEX);


    const std::string& getName() const;
    const ShaderAttributeTypeInfo& getType() const;
    std::uint32_t getBytesize() const;
    std::size_t getOffset() const;
    bool isNormalized() const;
    std::uint32_t getComponentCount() const;
    ShaderAttributeUpdateFrequency getUpdateFrequency() const;

    void setOffset(const std::size_t& offset);
    void setUpdateFrequency(ShaderAttributeUpdateFrequency frequency);

    private:
    std::string m_name{};
    ShaderAttributeTypeInfo m_shaderType;
    std::size_t m_offset{0llu};
    bool m_normalized{false};
    ShaderAttributeUpdateFrequency m_frequency{ShaderAttributeUpdateFrequency::EACH_VERTEX};
};

}  // namespace mono::gl
