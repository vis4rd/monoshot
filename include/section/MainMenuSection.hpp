#pragma once

#include "DebugSection.hpp"

class MainMenuSection final : public Section
{
    public:
    MainMenuSection();

    void update() noexcept override;
    void render() noexcept override;

    private:
};
