#pragma once

#include <cstdint>
#include <initializer_list>
#include <vector>

#include "ShaderAttribute.hpp"

namespace mono::gl
{

class ShaderAttributeLayout
{
    public:
    ShaderAttributeLayout() = default;
    ShaderAttributeLayout(std::initializer_list<ShaderAttribute> attributes);

    std::uint32_t getStride() const;
    std::vector<ShaderAttribute>& getAttributes();

    std::vector<ShaderAttribute>::iterator begin();
    std::vector<ShaderAttribute>::iterator end();
    std::vector<ShaderAttribute>::const_iterator begin() const;
    std::vector<ShaderAttribute>::const_iterator end() const;

    private:
    void calculateOffsetAndStride();

    private:
    std::vector<ShaderAttribute> m_attributes;
    std::uint32_t m_stride = 0u;
};


}  // namespace mono::gl
