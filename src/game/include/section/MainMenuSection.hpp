#pragma once

#include <section/Section.hpp>
#include <ui/Font.hpp>

#include "../ui/MainMenuLayout.hpp"

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
