#pragma once

#include <cstdint>
#include <queue>

class FramerateLimiter final
{
    public:
    FramerateLimiter();

    void setLimit(const std::int32_t& new_limit);
    const std::int32_t& getLimit() const;

    void wait();

    private:
    std::int32_t m_limit = 60;
    double m_frameTimeLimit = 1.0 / (m_limit + 0.0);
    const double& m_deltaTime;
};
