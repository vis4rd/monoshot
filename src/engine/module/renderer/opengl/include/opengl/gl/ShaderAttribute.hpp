#pragma once

#include <cstdint>
#include <string>

#include "ShaderAttributeType.hpp"

namespace mono::gl
{

/**
 * @brief Representation of OpenGL shader attribute. Intended for use with ShaderAttributeLayout.
 */
class ShaderAttribute
{
    public:
    /**
     * @brief Construct a new Shader Attribute object.
     * @param type Specification of the attribute type. Use one of the predefined ones in
     *             'ShaderAttributeType.hpp'.
     * @param name Name of the attribute in the shader.
     */
    ShaderAttribute(const ShaderAttributeTypeInfo& type, const std::string& name);

    /**
     * @brief Construct a new Shader Attribute object. With specification of normalization and
     *        offset.
     * @param type Specification of the attribute type. Use one of the predefined ones in
     *             'ShaderAttributeType.hpp'.
     * @param name Name of the attribute in the shader.
     * @param normalized True if the attribute is normalized, false otherwise.
     * @param offset Relative offset of the attribute in the buffer in regards to the VAO binding.
     *               Defaults to 0.
     * @note Offset is later calculated by the ShaderAttributeLayout, however it is possible to use
     *       this class without ShaderAttributeLayout.
     */
    ShaderAttribute(
        const ShaderAttributeTypeInfo& type,
        const std::string& name,
        bool normalized,
        std::size_t offset = 0llu);

    /**
     * @brief Construct a new Shader Attribute object. With specification of normalization and
     *        offset.
     * @param type Specification of the attribute type. Use one of the predefined ones in
     *             'ShaderAttributeType.hpp'.
     * @param name Name of the attribute in the shader.
     * @param offset Relative offset of the attribute in the buffer in regards to the VAO binding.
     * @param normalized True if the attribute is normalized, false otherwise. Defaults to false.
     * @note Offset is later calculated by the ShaderAttributeLayout, however it is possible to use
     *       this class without ShaderAttributeLayout.
     */
    ShaderAttribute(
        const ShaderAttributeTypeInfo& type,
        const std::string& name,
        std::size_t offset,
        bool normalized = false);


    const std::string& getName() const;

    /**
     * @brief Get the specification of attribute type.
     */
    const ShaderAttributeTypeInfo& getType() const;

    /**
     * @brief Get the total bytesize of the attribute, including number of values in attribute and
     *        number of components.
     */
    std::uint32_t getBytesize() const;
    std::size_t getOffset() const;
    bool isNormalized() const;

    /**
     * @brief Get the component count of the attribute. Equal to 1 for scalars, 3/4 for 3x3/4x4
     *        matrices. It specifies how many actual attributes should this attribute be split to.
     */
    std::uint32_t getComponentCount() const;

    void setOffset(const std::size_t& offset);

    private:
    std::string m_name{};
    ShaderAttributeTypeInfo m_shaderType;
    std::size_t m_offset{0llu};
    bool m_normalized{false};
};

}  // namespace mono::gl
