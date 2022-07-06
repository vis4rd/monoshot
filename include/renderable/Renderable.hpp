#pragma once

#include <concepts>

class Renderable
{
    protected:
    Renderable() = default;
    Renderable(const Renderable&) = default;
    Renderable(Renderable&&) = default;
    virtual Renderable& operator=(const Renderable&) = default;
    virtual Renderable& operator=(Renderable&&) = default;
    virtual ~Renderable() = default;

    protected:
    virtual void render() noexcept = 0;
};

template<typename T>
concept CRenderable = std::derived_from<std::remove_cvref_t<T>, Renderable>;

template<typename... Pack>
concept CRenderablePack = (CRenderable<Pack> && ...);
