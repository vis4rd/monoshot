#pragma once

namespace mono::gl
{

/**
 * @brief Verbose representation of OpenGL divisor for shader attribute. Intended for use with
 *        VertexArray when passing VertexBuffer to it.
 */
enum class ShaderAttributeUpdateFrequency
{
    EACH_VERTEX = 0,
    EACH_INSTANCE = 1,
};

}  // namespace mono::gl
