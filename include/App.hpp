#pragma once

#include "section/MainMenuSection.hpp"
#include "../meta/Version.hpp"
#include "ui/styles/MainMenuStyle.hpp"

class App
{
    public:
    App(const std::string& window_title, uint32_t width, uint32_t height);
    virtual ~App() noexcept;

    static void initLogger() noexcept;

    Window& getWindow();
    const Window& getWindow() const;

    void run() noexcept;

    private:
    void terminate(int code = 0) noexcept;

    private:
    Window& m_window;
    InputManager& m_input;
    SectionManager& m_sectionManager;
};
