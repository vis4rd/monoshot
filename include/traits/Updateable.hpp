#pragma once

#include <concepts>

class Updateable
{
    protected:
    Updateable() = default;
    Updateable(const Updateable&) = default;
    Updateable(Updateable&&) = default;
    virtual Updateable& operator=(const Updateable&) = default;
    virtual Updateable& operator=(Updateable&&) = default;
    virtual ~Updateable() = default;

    protected:
    virtual void update() noexcept = 0;
};

template<typename T>
concept CUpdateable = std::derived_from<std::remove_cvref_t<T>, Updateable>;

template<typename... Pack>
concept CUpdateablePack = (CUpdateable<Pack> && ...);
