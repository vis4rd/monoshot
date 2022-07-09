#pragma once

#include "DebugSection.hpp"

class MainMenuSection final : public Section
{
    public:
    MainMenuSection();

    void update() noexcept;
    void render() noexcept override;

    private:
};
