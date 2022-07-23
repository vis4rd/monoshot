#pragma once

#include "NewGameSection.hpp"

class MainMenuSection final : public Section
{
    public:
    MainMenuSection();

    void update() noexcept override;
    void render() noexcept override;

    private:
    UI::MainMenuLayout m_layout;
};
