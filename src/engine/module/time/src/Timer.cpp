#include "../include/time/Timer.hpp"

#include <GLFW/glfw3.h>

double Timer::getTotalTime()
{
    return glfwGetTime();
}

void Timer::restart()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_deltaTime = 0;
}

void Timer::update()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    double new_total = Timer::getTotalTime();
    m_deltaTime = new_total - m_lastTotalTime;
    m_lastTotalTime = new_total;
}

const double& Timer::deltaTime()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    return m_deltaTime;
}
