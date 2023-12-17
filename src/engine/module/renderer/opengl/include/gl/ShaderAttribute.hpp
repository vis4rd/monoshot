#pragma once

#include <cstdint>
#include <string>

#include "ShaderAttributeType.hpp"

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
        std::size_t offset = 0llu);
    ShaderAttribute(
        const ShaderAttributeTypeInfo& type,
        const std::string& name,
        std::size_t offset,
        bool normalized = false);


    const std::string& getName() const;
    const ShaderAttributeTypeInfo& getType() const;
    std::uint32_t getBytesize() const;
    std::size_t getOffset() const;
    bool isNormalized() const;
    std::uint32_t getComponentCount() const;

    void setOffset(const std::size_t& offset);

    private:
    std::string m_name{};
    ShaderAttributeTypeInfo m_shaderType;
    std::size_t m_offset{0llu};
    bool m_normalized{false};
};

}  // namespace mono::gl
