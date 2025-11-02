#pragma once

#include <memory>

#include <glbinding/gl/types.h>
#include <glm/fwd.hpp>

#include "RenderTargetTrait.hpp"
#include "opengl/gl/FrameBuffer.hpp"

namespace mono
{
class RenderTexture final : public RenderTarget
{
    public:
    RenderTexture(::gl::GLsizei width, ::gl::GLsizei height);
    RenderTexture(const RenderTexture& copy) = delete;
    RenderTexture(RenderTexture&& move) = default;
    ~RenderTexture() = default;

    RenderTexture& operator=(const RenderTexture& copy) = delete;
    RenderTexture& operator=(RenderTexture&& move) = default;

    void setSize(::gl::GLsizei width, ::gl::GLsizei height);
    glm::ivec2 getSize() const;

    void activate() const override;
    void deactivate() const override;

    ::gl::GLuint getID() const;

    private:
    std::unique_ptr<gl::FrameBuffer> m_framebuffer{nullptr};
};

static_assert(mono::RenderTargetTrait<mono::RenderTexture>);
}  // namespace mono
