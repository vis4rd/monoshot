#pragma once

#include <glm/glm.hpp>

#include "../utility/ResourceManager.hpp"

namespace ecs::component
{
// clang-format off

namespace impl
{
    template <typename T>
    class Scalar
    {
        public:
        Scalar(const T& val) : data(val)         { }
        
        Scalar() : data(static_cast<T>(0))                  { }
        Scalar(const Scalar& copy) : data(copy.data)        { }
        Scalar(Scalar&& move) : data(std::move(move.data))  { }
        ~Scalar() = default;

        operator T&()               { return data; }
        operator const T&() const   { return data; }
        operator T() const          { return data; }

        Scalar& operator=(const Scalar& copy)   { this->data = copy.data; return *this; }
        Scalar& operator=(Scalar&& move)        { this->data = std::move(move.data); return *this; }
        Scalar& operator=(const T& val)         { this->data = std::move(val); return *this; }

        Scalar operator+(const Scalar& rhs) const { Scalar retval; retval.data = this->data + rhs.data; return retval; }
        Scalar operator-(const Scalar& rhs) const { Scalar retval; retval.data = this->data - rhs.data; return retval; }
        Scalar operator/(const Scalar& rhs) const { Scalar retval; retval.data = this->data / rhs.data; return retval; }
        Scalar operator*(const Scalar& rhs) const { Scalar retval; retval.data = this->data * rhs.data; return retval; }
        Scalar operator+(const T& rhs)      const { Scalar retval; retval.data = this->data + rhs; return retval; }
        Scalar operator-(const T& rhs)      const { Scalar retval; retval.data = this->data - rhs; return retval; }
        Scalar operator/(const T& rhs)      const { Scalar retval; retval.data = this->data / rhs; return retval; }
        Scalar operator*(const T& rhs)      const { Scalar retval; retval.data = this->data * rhs; return retval; }

        Scalar& operator+=(const Scalar& rhs)   { this->data += rhs.data; return *this; }
        Scalar& operator-=(const Scalar& rhs)   { this->data -= rhs.data; return *this; }
        Scalar& operator/=(const Scalar& rhs)   { this->data /= rhs.data; return *this; }
        Scalar& operator*=(const Scalar& rhs)   { this->data *= rhs.data; return *this; }
        Scalar& operator+=(const T& rhs)        { this->data += rhs; return *this; }
        Scalar& operator-=(const T& rhs)        { this->data -= rhs; return *this; }
        Scalar& operator/=(const T& rhs)        { this->data /= rhs; return *this; }
        Scalar& operator*=(const T& rhs)        { this->data *= rhs; return *this; }

        public:
        T data;
    };
}

struct size : public glm::vec2 { };
struct position : public glm::vec2 { };
struct rotation : public impl::Scalar<float> { };
struct velocity : public impl::Scalar<float> { };
struct direction : public glm::vec2 { };  // direction of movement in degrees
struct collision : public impl::Scalar<bool> { };
struct max_velocity : public impl::Scalar<float> { max_velocity(float val = 7.f) : impl::Scalar<float>(val) { } };
struct acceleration : public impl::Scalar<float> { acceleration(float val = 100.f) : impl::Scalar<float>(val) { } };
struct is_bullet{};
struct lifetime
{
    lifetime(double timestamp, double life_duration = 1.0) : creation(timestamp), timeTillDeath(life_duration) {}
    double creation;
    double timeTillDeath;
};

// clang-format on
}  // namespace ecs::component
