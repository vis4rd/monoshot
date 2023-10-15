#pragma once

#include <memory>

#include "Texture.hpp"

namespace Texture
{

namespace impl
{

class Animation : public Texture
{
    public:
    Animation();
    Animation(
        const std::string_view& texture_file_path,
        const double& frame_duration,
        const TextureData& texture_data = TextureData());
    Animation(const Animation& copy);
    Animation(Animation&& move) noexcept;
    ~Animation() override;

    Animation& operator=(const Animation& copy);
    Animation& operator=(Animation&& move) noexcept;

    void setFrameDuration(const double& frame_duration);

    bool nextFrame();
    void resetFrame();

    protected:
    using Texture::nextSub;
    using Texture::resetSub;

    private:
    bool canChangeFrame();

    private:
    double m_frameDuration = 0.1;  // in seconds
    double m_lastFrameChangeTimestamp = 0.0;  // if m_lastFrameChangeTimestamp + m_useDelay <
                                              // Timer::getTotalTime() then it can be used again
};

}  // namespace impl

using Animation = std::shared_ptr<impl::Animation>;

}  // namespace Texture
