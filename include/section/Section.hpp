#pragma once

#include <string>

#include "../input/Input.hpp"
#include "../traits/Renderable.hpp"
#include "../traits/Updateable.hpp"

class Section : public Renderable, public Updateable
{
    public:
    void update() noexcept override = 0;
    void render() noexcept override = 0;

    protected:
    const std::string m_name = "Section";
};

template<typename T>
concept CSection = std::derived_from<std::remove_cvref_t<T>, Section>;

template<typename... Pack>
concept CSectionPack = (CSection<Pack> && ...);
