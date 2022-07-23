#pragma once

#include "SettingsSection.hpp"

class NewGameSection final : public Section
{
    public:
    NewGameSection();

    void update() noexcept override;
    void render() noexcept override;
};
