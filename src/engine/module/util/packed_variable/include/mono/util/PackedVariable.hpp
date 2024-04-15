#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <limits>
#include <ostream>
#include <tuple>

namespace mono::util
{

using PackedVariableSection = std::size_t;

#define _pv_runtime_assert(expression, fmt, ...) \
    (assert((expression) || !fprintf(stderr, ("Assertion message: " fmt "\n"), ##__VA_ARGS__)))

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

template<typename T = std::uint32_t, PackedVariableSection... Sections>
concept ValidSections = ((Sections + ...) <= (sizeof(T) * 8)) and ((Sections > 0) and ...);

template<std::size_t Index, PackedVariableSection... Sections>
concept ValidIndex = (Index < sizeof...(Sections)) and (Index >= 0);

template<std::size_t IndicesSize, std::size_t ArgsSize, PackedVariableSection... Sections>
concept ValidArgs = ((IndicesSize == ArgsSize) and (IndicesSize <= sizeof...(Sections)));

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

template<std::integral T, PackedVariableSection... Sections>
requires(detail::ValidSections<T, Sections...> and workaround::WorkingCompiler<T>)
class PackedVariable final
{
    public:
    using value_type = T;

    public:
    constexpr explicit PackedVariable(std::convertible_to<T> auto&&... values)
    requires(sizeof...(Sections) >= sizeof...(values));

    template<std::size_t Index>
    requires detail::ValidIndex<Index, Sections...>
    constexpr void set(std::convertible_to<value_type> auto value);

    template<std::size_t Index>
    requires detail::ValidIndex<Index, Sections...>
    constexpr value_type at() const;

    constexpr value_type get() const;

    friend constexpr std::ostream& operator<<(std::ostream& os, PackedVariable<T, Sections...> pv)
    {
        os << "0b" << std::bitset<sizeof(value_type) * 8>(pv.m_value);
        return os;
    }

    private:
    template<std::size_t Index>
    requires detail::ValidIndex<Index, Sections...>
    constexpr value_type left_padding() const;

    template<std::size_t Index>
    requires detail::ValidIndex<Index, Sections...>
    constexpr value_type right_padding() const;

    template<std::size_t... Indices, typename... Args>
    requires detail::ValidArgs<sizeof...(Indices), sizeof...(Args), Sections...>
    constexpr void set_sections_on_construction(std::index_sequence<Indices...>, Args&&... args);

    private:
    value_type m_value{};
};

template<std::integral T, PackedVariableSection... Sections>
requires(detail::ValidSections<T, Sections...> and workaround::WorkingCompiler<T>)
constexpr PackedVariable<T, Sections...>::PackedVariable(std::convertible_to<T> auto&&... values)
requires(sizeof...(Sections) >= sizeof...(values))
{
    this->set_sections_on_construction(std::make_index_sequence<sizeof...(values)>(), values...);
}

template<std::integral T, PackedVariableSection... Sections>
requires(detail::ValidSections<T, Sections...> and workaround::WorkingCompiler<T>)
template<std::size_t Index>
requires detail::ValidIndex<Index, Sections...>
constexpr void PackedVariable<T, Sections...>::set(std::convertible_to<value_type> auto value)
{
    _pv_runtime_assert(value >= 0, "Value %d must be greater or equal to 0", value);
    _pv_runtime_assert(
        value <= ((1 << util::valueAt<Index>(Sections...)) - 1),
        "Value %d out of range in section %d (bitsize is %d while max allowed is %d)",
        value,
        Index,
        (static_cast<std::uint32_t>(std::log2(value)) + 1),
        util::valueAt<Index>(Sections...));

    const auto left_shift = this->right_padding<Index>();
    constexpr value_type width = util::valueAt<Index>(Sections...);
    constexpr value_type mask = (1 << width) - 1;

    m_value |= ((value & mask) << left_shift);
}

template<std::integral T, PackedVariableSection... Sections>
requires(detail::ValidSections<T, Sections...> and workaround::WorkingCompiler<T>)
template<std::size_t Index>
requires detail::ValidIndex<Index, Sections...>
constexpr T PackedVariable<T, Sections...>::at() const
{
    const auto right_shift = this->right_padding<Index>();
    constexpr value_type width = util::valueAt<Index>(Sections...);
    constexpr value_type mask = (1 << width) - 1;

    return ((m_value >> right_shift) & mask);
}

template<std::integral T, PackedVariableSection... Sections>
requires(detail::ValidSections<T, Sections...> and workaround::WorkingCompiler<T>)
constexpr T PackedVariable<T, Sections...>::get() const
{
    return m_value;
}

template<std::integral T, PackedVariableSection... Sections>
requires(detail::ValidSections<T, Sections...> and workaround::WorkingCompiler<T>)
template<std::size_t Index>
requires detail::ValidIndex<Index, Sections...>
constexpr T PackedVariable<T, Sections...>::left_padding() const
{
    constexpr auto sum = []<std::size_t... Indices, typename... Args>(
                             std::index_sequence<Indices...>,
                             Args&&... args) -> value_type {
        constexpr auto value_below_index =
            []<typename Arg>(std::size_t index, Arg&& arg) -> value_type {
            return (index >= Index) ? 0 : arg;
        };
        return (value_below_index(Indices, std::forward<Args>(args)) + ...);
    };

    return sum(std::make_index_sequence<sizeof...(Sections)>(), Sections...);
}

template<std::integral T, PackedVariableSection... Sections>
requires(detail::ValidSections<T, Sections...> and workaround::WorkingCompiler<T>)
template<std::size_t Index>
requires detail::ValidIndex<Index, Sections...>
constexpr T PackedVariable<T, Sections...>::right_padding() const
{
    constexpr auto total_width = sizeof(value_type) * 8;
    constexpr value_type width = util::valueAt<Index>(Sections...);
    return total_width - this->left_padding<Index>() - width;
}

template<std::integral T, PackedVariableSection... Sections>
requires(detail::ValidSections<T, Sections...> and workaround::WorkingCompiler<T>)
template<std::size_t... Indices, typename... Args>
requires detail::ValidArgs<sizeof...(Indices), sizeof...(Args), Sections...>
constexpr void PackedVariable<T, Sections...>::set_sections_on_construction(
    std::index_sequence<Indices...>,
    Args&&... args)
{
    (this->set<Indices>(args), ...);
}

}  // namespace mono::util
