#pragma once

#include <concepts>
#include <cstdint>
#include <iterator>

#include <glm/ext/matrix_float3x3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

namespace mono::gl
{

template<typename T, typename E>
concept ContiguousContainerTrait = requires(T t) {
    {
        t.begin()
    } -> std::contiguous_iterator;
    {
        t[0]
    } -> std::same_as<E&>;
    {
        t.size()
    } -> std::integral;
    {
        t.data()
    } -> std::same_as<E*>;
};

template<typename T, typename E>
concept TwoElementVariableTrait = requires(T t) {
    {
        t.x
    } -> std::same_as<E&>;
    {
        t.y
    } -> std::same_as<E&>;
};

template<typename T, typename E>
concept ThreeElementVariableTrait = TwoElementVariableTrait<T, E> and requires(T t) {
    {
        t.z
    } -> std::same_as<E&>;
};

template<typename T, typename E>
concept FourElementVariableTrait = ThreeElementVariableTrait<T, E> and requires(T t) {
    {
        t.w
    } -> std::same_as<E&>;
};

template<typename T>
concept GlmMatrixVariableTrait = std::same_as<T, glm::mat3> or std::same_as<T, glm::mat4>;

template<typename T>
concept ValidUniformVariableTrait =
    ContiguousContainerTrait<T, std::int32_t> or ContiguousContainerTrait<T, std::uint32_t>
    or TwoElementVariableTrait<T, std::int32_t> or TwoElementVariableTrait<T, float>
    or ThreeElementVariableTrait<T, std::int32_t> or ThreeElementVariableTrait<T, float>
    or FourElementVariableTrait<T, std::int32_t> or FourElementVariableTrait<T, float>
    or GlmMatrixVariableTrait<T> or std::same_as<T, std::int32_t> or std::same_as<T, std::uint32_t>
    or std::same_as<T, float> or std::same_as<T, bool>;

}  // namespace mono::gl
