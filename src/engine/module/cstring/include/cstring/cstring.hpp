#pragma once

#include <stdexcept>
#include <string_view>
#include <array>
#include <concepts>

namespace vis
{

template<std::size_t N>
class cstring final
{
    private:
    using underlying_type = std::array<const char, N>;

    public:
    using value_type = typename underlying_type::value_type;
    using size_type = typename underlying_type::size_type;
    using difference_type = typename underlying_type::difference_type;
    using pointer = typename underlying_type::pointer;
    using const_pointer = typename underlying_type::const_pointer;
    using reference = typename underlying_type::reference;
    using const_reference = typename underlying_type::const_reference;

    using iterator = typename underlying_type::iterator;
    using const_iterator = typename underlying_type::const_iterator;
    using reverse_iterator = typename underlying_type::reverse_iterator;
    using const_reverse_iterator = typename underlying_type::const_reverse_iterator;

    public:
    constexpr explicit cstring(const auto... cs)
        : m_data{cs...}
    { }

    constexpr cstring(const char (&str)[N])
        : cstring{str, std::make_integer_sequence<size_type, N>{}}
    { }

    [[nodiscard]] constexpr const_reference operator[](size_type i) const { return m_data[i]; }

    [[nodiscard]] constexpr const_reference front() const { return m_data.front(); }

    [[nodiscard]] constexpr const_reference back() const { return m_data.back(); }

    [[nodiscard]] constexpr size_type length() const { return m_data.length(); }

    [[nodiscard]] constexpr std::size_t size() const { return m_data.size(); }

    template<std::integral I>
    [[nodiscard]] constexpr const_reference at(I&& n) const
    {
        if((n < 0) or (n > N))
        {
            throw std::out_of_range("cstring::at: i (which is " + std::to_string(n)
                                    + ") >= N (which is " + std::to_string(N) + ")");
        }
        return m_data.at(n);
    }

    [[nodiscard]] constexpr bool empty() const { return m_data.empty(); }

    [[nodiscard]] constexpr const auto& data() const { return m_data; }

    [[nodiscard]] constexpr auto begin() { return m_data.begin(); }

    [[nodiscard]] constexpr auto end() { return m_data.end(); }

    [[nodiscard]] constexpr auto cbegin() const { return m_data.cbegin(); }

    [[nodiscard]] constexpr auto cend() const { return m_data.cend(); }

    [[nodiscard]] constexpr auto rbegin() { return m_data.rbegin(); }

    [[nodiscard]] constexpr auto rend() { return m_data.rend(); }

    [[nodiscard]] constexpr auto crbegin() const { return m_data.crbegin(); }

    [[nodiscard]] constexpr auto crend() const { return m_data.crend(); }

    [[nodiscard]] constexpr auto operator<=>(const cstring&) const = default;
    [[nodiscard]] constexpr bool operator==(const cstring&) const = default;
    [[nodiscard]] constexpr bool operator!=(const cstring&) const = default;
    [[nodiscard]] constexpr bool operator<(const cstring&) const = default;
    [[nodiscard]] constexpr bool operator>(const cstring&) const = default;
    [[nodiscard]] constexpr bool operator<=(const cstring&) const = default;
    [[nodiscard]] constexpr bool operator>=(const cstring&) const = default;

    [[nodiscard]] constexpr operator std::string_view() const { return {std::data(m_data), N}; }


    private:
    template<size_type... I>
    constexpr cstring(const value_type (&str)[N], std::integer_sequence<size_type, I...>)
        : m_data{str[I]...}
    { }

    private:
    underlying_type m_data{};
};

}  // namespace vis
