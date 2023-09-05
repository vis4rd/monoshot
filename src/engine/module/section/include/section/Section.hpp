#pragma once

#include <string>

#include "traits/Renderable.hpp"
#include "traits/Updateable.hpp"

class Section
{
    public:
    const std::string& name() const noexcept;
    virtual void update() noexcept = 0;
    virtual void render() noexcept = 0;

    protected:
    std::string m_name = "Section";
};

template<typename T>
concept IsSectionTrait = std::derived_from<std::remove_cvref_t<T>, Section>;
