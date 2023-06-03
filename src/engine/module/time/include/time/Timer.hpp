#pragma once

#include <mutex>

class Timer
{
    public:
    [[nodiscard]] static double getTotalTime();
    void restart();  // set deltaTime to 0
    void update();  // calc deltaTime

    const double& deltaTime();

    private:
    double m_deltaTime = 0.0;
    double m_lastTotalTime = 0.0;
    std::mutex m_mutex{};
};
