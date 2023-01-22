#pragma once

#include "Section.hpp"
#include "../ui/layouts/SettingsLayout.hpp"
#include "../ui/layouts/LowerNavigationBoxLayout.hpp"

class SettingsSection final : public Section
{
    public:
    SettingsSection();

    void update() noexcept override;
    void render() noexcept override;

    private:
    UI::SettingsLayout m_layout;
    UI::LowerNavigationBoxLayout m_navLayout;
};
