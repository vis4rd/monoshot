#pragma once

#include <string>

#include "../input/InputManager.hpp"
#include "../traits/Renderable.hpp"
#include "../traits/Updateable.hpp"

class Section : public Renderable, public Updateable
{
    public:
    const std::string& name() const noexcept;
    void update() noexcept override = 0;
    void render() noexcept override = 0;

    protected:
    std::string m_name = "Section";
};

template<typename T>
concept CSection = std::derived_from<std::remove_cvref_t<T>, Section>;

template<typename... Pack>
concept CSectionPack = (CSection<Pack> && ...);
