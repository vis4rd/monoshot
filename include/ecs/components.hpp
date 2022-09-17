#pragma once

#include <glm/glm.hpp>

namespace ecs::component
{
// clang-format off

namespace impl
{
    template <typename T>
    class ImplicitConversionsInterface
    {
        public:
        ImplicitConversionsInterface(const T& val) : data(val) { }
        ImplicitConversionsInterface() : ImplicitConversionsInterface(static_cast<T>(0)) { }
        operator T&() { return data; }
        operator const T&() const { return data; }
        operator T() const { return data; }

        public:
        T data;
    };
    // TODO: replace this with 'scalar' base class with all operators overloaded
}

struct position : public glm::vec2
{

};

struct rotation : public impl::ImplicitConversionsInterface<float>
{

};

struct velocity : public impl::ImplicitConversionsInterface<float>
{

};

struct max_velocity : public impl::ImplicitConversionsInterface<float>
{
    max_velocity() : impl::ImplicitConversionsInterface<float>(7.f) { }
};

struct acceleration : public impl::ImplicitConversionsInterface<float>
{
    acceleration() : impl::ImplicitConversionsInterface<float>(100.f) { }
};

struct direction : public impl::ImplicitConversionsInterface<float> // direction of movement in degrees
{

};

// clang-format on
}  // namespace ecs::component
