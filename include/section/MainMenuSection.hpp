#pragma once

#include "SettingsSection.hpp"

class MainMenuSection final : public Section
{
    public:
    MainMenuSection();

    void update() noexcept override;
    void render() noexcept override;

    private:
    UI::BaseUiLayout m_layout;
};
