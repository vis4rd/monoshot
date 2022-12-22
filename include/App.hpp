#pragma once

#include "section/MainMenuSection.hpp"
#include "../meta/Version.hpp"
#include "ui/styles/MainMenuStyle.hpp"
#include "utility/Timer.hpp"
#include "utility/FramerateLimiter.hpp"

class App
{
    public:
    App(const std::string& window_title, uint32_t width, uint32_t height);
    virtual ~App() noexcept;

    static void initLogger() noexcept;
    void initTextures() noexcept;
    void initFonts() noexcept;

    Window& getWindow();
    const Window& getWindow() const;

    void run() noexcept;

    private:
    void terminate(int code = 0) noexcept;
    void destroyTextures() noexcept;
    void destroyFonts() noexcept;

    private:
    std::shared_ptr<Window> m_window;
    std::shared_ptr<Timer> m_timer;
    std::shared_ptr<FramerateLimiter> m_limiter;
    InputManager& m_input;
    SectionManager& m_sectionManager;
};
