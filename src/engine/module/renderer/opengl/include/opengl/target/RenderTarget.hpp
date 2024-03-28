#pragma once

#include <glad/gl.h>

#include "../gl/FrameBuffer.hpp"
#include "../gl/VertexArray.hpp"

namespace mono::gl
{
class RenderTarget
{
    public:
    RenderTarget(GLsizei width, GLsizei height);
    RenderTarget(const RenderTarget& copy) = delete;
    RenderTarget(RenderTarget&& move) = default;
    ~RenderTarget() = default;

    RenderTarget& operator=(const RenderTarget& copy) = delete;
    RenderTarget& operator=(RenderTarget&& move) = default;

    void setSize(GLsizei width, GLsizei height);
    glm::ivec2 getSize() const;

    /**
     * Activate the RenderTarget.
     *
     * This function sets the RenderTarget as the active target for rendering operations. After
     * calling this function, any subsequent submitted draw calls will be directed to this
     * RenderTarget.
     */
    void activate() const;
    void deactivate() const;

    /**
     * Render the contents of this RenderTarget to the other target using the specified shader.
     *
     * @param shader_name The name of the shader to use for rendering.
     *
     * The target which this RenderTarget will be rendered to has to be specified before calling
     * this function.
     *
     * @important Fragment shader in the specified shader program must declare `sampler2D` uniform
     *            variable at location `0` to be bound at unit `0`.
     */
    void render(std::string_view shader_name) const;

    /**
     * Render the contents of this RenderTarget to the other target with shader activated
     * externally.
     *
     * The target which this RenderTarget will be rendered to has to be specified before calling
     * this function.
     *
     * @note The user must call `glUseProgram` with the desired shader program before calling this.
     * @important Fragment shader in the specified shader program must declare `sampler2D` uniform
     *            variable at location `0` to be bound at unit `0`.
     */
    void render() const;

    protected:
    RenderTarget() = default;

    protected:
    mono::gl::FrameBuffer m_framebuffer;
    mono::gl::VertexArray m_vao;
};

}  // namespace mono::gl
