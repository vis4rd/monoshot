#pragma once

#include <concepts>
#include <utility>

template<typename T>
concept NumericC = std::integral<T> or std::floating_point<T>;

// clang-format off
template<NumericC T>
class Scalar
{
    public:
    // NOLINTNEXTLINE(google-explicit-constructor)
    constexpr Scalar(const T& val) : data(val)       { }
    // NOLINTNEXTLINE(google-explicit-constructor)
    constexpr Scalar(T&& val) : data(std::move(val)) { }

    constexpr Scalar() : data(static_cast<T>(0))                 { }
    constexpr Scalar(const Scalar& copy) : data(copy.data)       { }
    constexpr Scalar(Scalar&& move) noexcept : data(std::move(move.data)) { }
    constexpr ~Scalar() = default;

    // NOLINTNEXTLINE(google-explicit-constructor)
    operator T&()             { return data; }
    // NOLINTNEXTLINE(google-explicit-constructor)
    operator const T&() const { return data; }
    // NOLINTNEXTLINE(google-explicit-constructor)
    operator T()        const { return data; }

    constexpr Scalar& operator=(const Scalar& copy)     { this->data = copy.data; return *this; }
    constexpr Scalar& operator=(Scalar&& move) noexcept { this->data = std::move(move.data); return *this; }
    constexpr Scalar& operator=(const T& val)           { this->data = std::move(val); return *this; }
    constexpr Scalar& operator=(T&& val)                { this->data = std::move(val); return *this; }

    constexpr Scalar operator+(const Scalar& rhs) const { Scalar retval; retval.data = this->data + rhs.data; return retval; }
    constexpr Scalar operator-(const Scalar& rhs) const { Scalar retval; retval.data = this->data - rhs.data; return retval; }
    constexpr Scalar operator/(const Scalar& rhs) const { Scalar retval; retval.data = this->data / rhs.data; return retval; }
    constexpr Scalar operator*(const Scalar& rhs) const { Scalar retval; retval.data = this->data * rhs.data; return retval; }
    constexpr Scalar operator+(const T& rhs)      const { Scalar retval; retval.data = this->data + rhs; return retval; }
    constexpr Scalar operator-(const T& rhs)      const { Scalar retval; retval.data = this->data - rhs; return retval; }
    constexpr Scalar operator/(const T& rhs)      const { Scalar retval; retval.data = this->data / rhs; return retval; }
    constexpr Scalar operator*(const T& rhs)      const { Scalar retval; retval.data = this->data * rhs; return retval; }

    constexpr Scalar& operator+=(const Scalar& rhs) { this->data += rhs.data; return *this; }
    constexpr Scalar& operator-=(const Scalar& rhs) { this->data -= rhs.data; return *this; }
    constexpr Scalar& operator/=(const Scalar& rhs) { this->data /= rhs.data; return *this; }
    constexpr Scalar& operator*=(const Scalar& rhs) { this->data *= rhs.data; return *this; }
    constexpr Scalar& operator+=(const T& rhs)      { this->data += rhs; return *this; }
    constexpr Scalar& operator-=(const T& rhs)      { this->data -= rhs; return *this; }
    constexpr Scalar& operator/=(const T& rhs)      { this->data /= rhs; return *this; }
    constexpr Scalar& operator*=(const T& rhs)      { this->data *= rhs; return *this; }

    constexpr bool operator==(const Scalar& rhs) const { return this->data == rhs.data; }
    constexpr bool operator!=(const Scalar& rhs) const { return this->data != rhs.data; }
    constexpr bool operator==(const T& rhs)      const { return this->data == rhs; }
    constexpr bool operator!=(const T& rhs)      const { return this->data != rhs; }

    public:
    T data;
};

// clang-format on
