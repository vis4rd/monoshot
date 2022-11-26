#include "../../include/audio/AudioDevice.hpp"

namespace raudio
{
#include <raudio.h>
}

AudioDevice::AudioDevice()
{
    raudio::InitAudioDevice();
}

AudioDevice::~AudioDevice()
{
    raudio::CloseAudioDevice();
}
