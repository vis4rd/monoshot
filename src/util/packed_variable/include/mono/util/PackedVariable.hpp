#pragma once

#include <bitset>
#include <cassert>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <cstdio>
#include <ostream>
#include <tuple>
#include <utility>

static_assert(true);
// TODO: fix this instead of ignoring format warnings
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat"

namespace mono::util
{

using PackedVariableSection = std::size_t;

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define PV_RUNTIME_ASSERT(expression, fmt, ...) \
    (assert(                                    \
        (expression)                            \
        || !fprintf(stderr, ("Assertion message: " fmt "\n")__VA_OPT__(, ) __VA_ARGS__)))

namespace util
{

template<size_t I, typename... TS>
requires((I < sizeof...(TS)) and (sizeof...(TS) > 0))
constexpr decltype(auto) valueAt(TS&&... ts) noexcept
{
    return std::get<I>(std::forward_as_tuple(std::forward<TS>(ts)...));
}

}  // namespace util

namespace detail
{

template<typename T = std::uint32_t, PackedVariableSection... SECTIONS>
concept ValidSections = ((SECTIONS + ...) <= (sizeof(T) * 8)) and ((SECTIONS > 0) and ...);

template<std::size_t INDEX, PackedVariableSection... SECTIONS>
concept ValidIndex = (INDEX < sizeof...(SECTIONS)) and (INDEX >= 0);

template<std::size_t INDICES_SIZE, std::size_t ARGS_SIZE, PackedVariableSection... SECTIONS>
concept ValidArgs = ((INDICES_SIZE == ARGS_SIZE) and (INDICES_SIZE <= sizeof...(SECTIONS)));

}  // namespace detail

namespace workaround
{

// Right shift on signed integers does not work correctly in some cases on compilers:
// - GCC 10 and maybe below (did not check),
// - Clang 14-16.
//
// MSVC has never been checked.
// Clang 17 and above have never been checked.

#ifdef PACKED_VARIABLE_ENABLE_COMPILER_CHECK

    #ifndef __clang_major__
        #define __clang_major__ 0
    #endif

    #ifndef __GNUC__
        #define __GNUC__ 0
    #endif

namespace detail
{
template<typename T>
concept WorkingGnuCompiler = std::signed_integral<T> and (__GNUC__ >= 11);

template<typename T>
concept WorkingClangCompiler =
    std::signed_integral<T> and (__clang_major__ <= 13) and (__clang_major__ > 0);
}  // namespace detail

template<typename T>
concept WorkingCompiler =
    std::unsigned_integral<T> or detail::WorkingGnuCompiler<T> or detail::WorkingClangCompiler<T>;

#else

template<typename T>
concept WorkingCompiler = true;

#endif

}  // namespace workaround

template<std::integral T, PackedVariableSection... SECTIONS>
requires(detail::ValidSections<T, SECTIONS...> and workaround::WorkingCompiler<T>)
class PackedVariable final
{
    public:
    using value_type = T;

    public:
    constexpr explicit PackedVariable(std::convertible_to<T> auto&&... values)
    requires(sizeof...(SECTIONS) >= sizeof...(values));

    template<std::size_t INDEX>
    requires detail::ValidIndex<INDEX, SECTIONS...>
    constexpr void set(std::convertible_to<value_type> auto value);

    template<std::size_t INDEX>
    requires detail::ValidIndex<INDEX, SECTIONS...>
    constexpr value_type at() const;

    constexpr value_type get() const;

    friend constexpr std::ostream& operator<<(std::ostream& os, PackedVariable<T, SECTIONS...> pv)
    {
        os << "0b" << std::bitset<sizeof(value_type) * 8>(pv.m_value);
        return os;
    }

    private:
    template<std::size_t INDEX>
    requires detail::ValidIndex<INDEX, SECTIONS...>
    constexpr value_type leftPadding() const;

    template<std::size_t INDEX>
    requires detail::ValidIndex<INDEX, SECTIONS...>
    constexpr value_type rightPadding() const;

    template<std::size_t... INDICES, typename... ARGS>
    requires detail::ValidArgs<sizeof...(INDICES), sizeof...(ARGS), SECTIONS...>
    constexpr void setSectionsOnConstruction(std::index_sequence<INDICES...>, ARGS&&... args);

    private:
    value_type m_value{};
};

template<std::integral T, PackedVariableSection... SECTIONS>
requires(detail::ValidSections<T, SECTIONS...> and workaround::WorkingCompiler<T>)
constexpr PackedVariable<T, SECTIONS...>::PackedVariable(std::convertible_to<T> auto&&... values)
requires(sizeof...(SECTIONS) >= sizeof...(values))
{
    this->setSectionsOnConstruction(std::make_index_sequence<sizeof...(values)>(), values...);
}

template<std::integral T, PackedVariableSection... SECTIONS>
requires(detail::ValidSections<T, SECTIONS...> and workaround::WorkingCompiler<T>)
template<std::size_t INDEX>
requires detail::ValidIndex<INDEX, SECTIONS...>
constexpr void PackedVariable<T, SECTIONS...>::set(std::convertible_to<value_type> auto value)
{
    PV_RUNTIME_ASSERT(value >= 0, "Value %d must be greater or equal to 0", value);
    PV_RUNTIME_ASSERT(
        value <= ((1 << util::valueAt<INDEX>(SECTIONS...)) - 1),
        "Value %d out of range in section %d (bitsize is %d while max allowed is %d)",
        value,
        INDEX,
        (static_cast<std::uint32_t>(std::log2(value)) + 1),
        util::valueAt<INDEX>(SECTIONS...));

    const auto left_shift = this->rightPadding<INDEX>();
    constexpr value_type width = util::valueAt<INDEX>(SECTIONS...);
    constexpr value_type mask = (1 << width) - 1;

    m_value |= ((value & mask) << left_shift);
}

template<std::integral T, PackedVariableSection... SECTIONS>
requires(detail::ValidSections<T, SECTIONS...> and workaround::WorkingCompiler<T>)
template<std::size_t INDEX>
requires detail::ValidIndex<INDEX, SECTIONS...>
constexpr T PackedVariable<T, SECTIONS...>::at() const
{
    const auto right_shift = this->rightPadding<INDEX>();
    constexpr value_type width = util::valueAt<INDEX>(SECTIONS...);
    constexpr value_type mask = (1 << width) - 1;

    return ((m_value >> right_shift) & mask);
}

template<std::integral T, PackedVariableSection... SECTIONS>
requires(detail::ValidSections<T, SECTIONS...> and workaround::WorkingCompiler<T>)
constexpr T PackedVariable<T, SECTIONS...>::get() const
{
    return m_value;
}

template<std::integral T, PackedVariableSection... SECTIONS>
requires(detail::ValidSections<T, SECTIONS...> and workaround::WorkingCompiler<T>)
template<std::size_t INDEX>
requires detail::ValidIndex<INDEX, SECTIONS...>
constexpr T PackedVariable<T, SECTIONS...>::leftPadding() const
{
    constexpr auto sum = []<std::size_t... INDICES, typename... ARGS>(
                             std::index_sequence<INDICES...>,
                             ARGS&&... args) -> value_type {
        constexpr auto value_below_index =
            []<typename ARG>(std::size_t index, ARG arg) -> value_type {
            return (index >= INDEX) ? 0 : arg;
        };
        return (value_below_index(INDICES, std::forward<ARGS>(args)) + ...);
    };

    return sum(std::make_index_sequence<sizeof...(SECTIONS)>(), SECTIONS...);
}

template<std::integral T, PackedVariableSection... SECTIONS>
requires(detail::ValidSections<T, SECTIONS...> and workaround::WorkingCompiler<T>)
template<std::size_t INDEX>
requires detail::ValidIndex<INDEX, SECTIONS...>
constexpr T PackedVariable<T, SECTIONS...>::rightPadding() const
{
    constexpr auto total_width = sizeof(value_type) * 8;
    constexpr value_type width = util::valueAt<INDEX>(SECTIONS...);
    return total_width - this->leftPadding<INDEX>() - width;
}

template<std::integral T, PackedVariableSection... SECTIONS>
requires(detail::ValidSections<T, SECTIONS...> and workaround::WorkingCompiler<T>)
template<std::size_t... INDICES, typename... ARGS>
requires detail::ValidArgs<sizeof...(INDICES), sizeof...(ARGS), SECTIONS...>
constexpr void PackedVariable<T, SECTIONS...>::setSectionsOnConstruction(
    std::index_sequence<INDICES...>,
    ARGS&&... args)
{
    (this->set<INDICES>(std::forward<ARGS>(args)), ...);
}

}  // namespace mono::util

#pragma GCC diagnostic pop
