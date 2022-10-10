#pragma once

#include "NewGameSection.hpp"
#include "../ui/fonts/Font.hpp"

class MainMenuSection final : public Section
{
    public:
    MainMenuSection();

    void update() noexcept override;
    void render() noexcept override;

    private:
    const std::string m_titleText = "MONOSHOT";
    UI::MainMenuLayout m_layout;
    Font m_titleFont;
};
