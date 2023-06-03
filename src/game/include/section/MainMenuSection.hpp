#pragma once

#include "Section.hpp"
#include "../ui/fonts/Font.hpp"
#include "../ui/layouts/MainMenuLayout.hpp"

class MainMenuSection final : public Section
{
    public:
    MainMenuSection();

    void update() noexcept override;
    void render() noexcept override;

    private:
    const std::string m_titleText = "MONOSHOT";
    UI::MainMenuLayout m_layout;
};
