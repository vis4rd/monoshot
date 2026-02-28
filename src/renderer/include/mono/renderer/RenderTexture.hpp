#pragma once

#include <memory>

#include <glbinding/gl/types.h>
#include <glm/fwd.hpp>

#include "RenderTarget.hpp"
#include "opengl/gl/FrameBuffer.hpp"

namespace mono
{
class RenderTexture final : public mono::renderer::RenderTarget
{
    public:
    RenderTexture(::gl::GLsizei width, ::gl::GLsizei height);
    RenderTexture(const RenderTexture& copy) = delete;
    RenderTexture(RenderTexture&& move) = default;
    ~RenderTexture() override = default;

    RenderTexture& operator=(const RenderTexture& copy) = delete;
    RenderTexture& operator=(RenderTexture&& move) = default;

    // RenderTarget interface
    void activate() const override;
    void deactivate() const override;
    glm::ivec2 getSize() const override;
    ::gl::GLuint getFramebufferHandle() const override;
    //

    void setSize(::gl::GLsizei width, ::gl::GLsizei height);

    ::gl::GLuint getID() const;

    private:
    std::unique_ptr<gl::FrameBuffer> m_framebuffer{nullptr};
};

static_assert(mono::renderer::RenderTargetTrait<mono::RenderTexture>);
}  // namespace mono
