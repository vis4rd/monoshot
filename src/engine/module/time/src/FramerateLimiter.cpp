#include "../include/time/FramerateLimiter.hpp"

#include <chrono>

#include <spdlog/spdlog.h>

#include "resource/ResourceManager.hpp"

FramerateLimiter::FramerateLimiter()
    : m_deltaTime(ResourceManager::timer->deltaTime())
{
    spdlog::info("Creating FramerateLimiter");
}

void FramerateLimiter::setLimit(const std::int32_t& new_limit)
{
    if(new_limit > 0)
    {
        m_limit = new_limit;
        m_frameTimeLimit = 1.0 / (m_limit + 0.0);
    }
}

const std::int32_t& FramerateLimiter::getLimit() const
{
    return m_limit;
}

void FramerateLimiter::wait()
{
    if(not ResourceManager::window->isVerticalSyncEnabled())
    {
        const double delay = std::max(m_frameTimeLimit - m_deltaTime, 0.0) * 2;
        // spdlog::trace("delta = {:.4f}, limit = {:.4f}, delay = {:.4f}", m_deltaTime,
        // m_frameTimeLimit, delay);

        //! we cannot trust the native system scheduler, because its precision is about 10-15ms, so
        //! sleeping is not an option
        // std::this_thread::sleep_for(std::chrono::duration<double>(delay / 2));

        const double next_frame_timestamp = Timer::getTotalTime() + delay;
        while(Timer::getTotalTime() < next_frame_timestamp)
        {
            // keep the cpu busy for a moment :)
            int something_to_delay = -1;
            something_to_delay++;
        }
    }
}
