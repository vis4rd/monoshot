#pragma once

#include <concepts>

#include <glbinding/gl/types.h>
#include <glm/fwd.hpp>

namespace mono::renderer
{
class RenderTarget
{
    public:
    RenderTarget() = default;
    RenderTarget(const RenderTarget&) = default;
    RenderTarget(RenderTarget&&) = default;
    virtual ~RenderTarget() = default;

    RenderTarget& operator=(const RenderTarget&) = default;
    RenderTarget& operator=(RenderTarget&&) = default;

    virtual void activate() const = 0;
    virtual void deactivate() const = 0;

    virtual glm::ivec2 getSize() const = 0;
    virtual ::gl::GLuint getFramebufferHandle() const = 0;
};

template<typename T>
concept RenderTargetTrait = std::derived_from<T, RenderTarget>;
}  // namespace mono::renderer
