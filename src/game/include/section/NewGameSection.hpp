#pragma once


#include <section/Section.hpp>

#include "../ui/LowerNavigationBoxLayout.hpp"
#include "../ui/NewGameLayout.hpp"

class NewGameSection final : public Section
{
    public:
    NewGameSection();

    void update() noexcept override;
    void render() noexcept override;

    private:
    UI::NewGameLayout m_layout;
    UI::LowerNavigationBoxLayout m_navLayout;
    bool m_showDebugLevel = false;
};
