#pragma once

#include <concepts>

#include <glbinding/gl/types.h>
#include <glm/fwd.hpp>

namespace mono::renderer
{
class RenderTarget
{
    public:
    virtual ~RenderTarget() = default;
    virtual void activate() const = 0;
    virtual void deactivate() const = 0;

    virtual glm::ivec2 getSize() const = 0;
    virtual ::gl::GLuint getFramebufferHandle() const = 0;
};

template<typename T>
concept RenderTargetTrait = std::derived_from<T, RenderTarget>;
}  // namespace mono::renderer
