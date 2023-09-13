#pragma once

#include <config/ConfigLoader.hpp>
#include <time/FramerateLimiter.hpp>
#include <time/Timer.hpp>
#include <ui/MainMenuStyle.hpp>

#include "section/MainMenuSection.hpp"

class App
{
    public:
    App(const std::string& window_title, uint32_t width, uint32_t height);
    virtual ~App() noexcept;

    static void initLogger() noexcept;
    void initTextures() noexcept;
    void initFonts() noexcept;
    void initAudio() noexcept;

    Window& getWindow();
    const Window& getWindow() const;

    void run() noexcept;

    private:
    void terminate(int code = 0) noexcept;
    void destroyTextures() noexcept;
    void destroyFonts() noexcept;

    private:
    mono::ConfigLoader& m_configLoader;
    std::shared_ptr<Window> m_window;
    std::shared_ptr<Timer> m_timer;
    std::shared_ptr<FramerateLimiter> m_limiter;
    InputManager& m_input;
    SectionManager& m_sectionManager;
};
