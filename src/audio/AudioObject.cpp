#include "../../include/audio/AudioObject.hpp"

namespace raudio
{
#include <raudio.h>
}

void AudioObject::setMasterVolume(const float& volume)
{
    raudio::SetMasterVolume(volume);
}
