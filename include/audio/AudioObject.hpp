#pragma once

#include "AudioDevice.hpp"

class AudioObject
{
    public:
    AudioObject() = default;
    virtual ~AudioObject() = default;

    static void setMasterVolume(const float& volume);

    protected:
    // static AudioDevice m_device;
};
