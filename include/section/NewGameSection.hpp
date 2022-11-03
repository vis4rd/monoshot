#pragma once


#include "Section.hpp"
#include "../ui/layouts/NewGameLayout.hpp"
#include "../ui/layouts/LowerNavigationBoxLayout.hpp"

class NewGameSection final : public Section
{
    public:
    NewGameSection();

    void update() noexcept override;
    void render() noexcept override;

    private:
    UI::NewGameLayout m_layout;
    UI::LowerNavigationBoxLayout m_navLayout;
};
