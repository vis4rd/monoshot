#include "../../include/texture/Animation.hpp"

#include "../../include/utility/Timer.hpp"

namespace Texture::impl
{

Animation::Animation()
    : Texture()
{
}

Animation::Animation(const std::string_view& texture_file_path, const TextureData& texture_data)
    : Texture(texture_file_path, texture_data)
{
}

Animation::Animation(const Animation& copy)
    : Texture(copy)
{
}

Animation::Animation(Animation&& move)
    : Texture(std::move(move))
{
}

Animation::~Animation() { }

Animation& Animation::operator=(const Animation& copy)
{
    this->Texture::operator=(copy);
    return *this;
}

Animation& Animation::operator=(Animation&& move)
{
    this->Texture::operator=(std::move(move));
    return *this;
}

bool Animation::nextFrame()
{
    if(this->canChangeFrame())
    {
        this->nextSub();
        m_lastFrameChangeTimestamp = Timer::getTotalTime();
        return true;
    }
    return false;
}

void Animation::resetFrame()
{
    this->resetSub();
}

bool Animation::canChangeFrame()
{
    return (m_lastFrameChangeTimestamp + m_frameDuration) < Timer::getTotalTime();
}

}  // namespace Texture::impl
