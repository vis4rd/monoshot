#pragma once

#include <concepts>

namespace mono
{
class RenderTarget
{
    public:
    virtual ~RenderTarget() = default;
    virtual void activate() const = 0;
    virtual void deactivate() const = 0;
};

template<typename T>
concept RenderTargetTrait = std::derived_from<T, RenderTarget>;
}  // namespace mono
