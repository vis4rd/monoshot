#pragma once

#include "AudioObject.hpp"

namespace raudio
{
#include <raudio.h>
}  // namespace raudio

class Sound : public AudioObject
{
    public:
    Sound();
    Sound(const std::string_view& filename);
    Sound(const char* filename);
    Sound(const Sound& copy);
    Sound(Sound&& move);
    ~Sound();

    Sound& operator=(const Sound& copy);
    Sound& operator=(Sound&& move);

    void setVolume(const float& volume);
    void setPitch(const float& pitch);
    void setPan(const float& pan);
    void play();
    void playMulti();
    void stop();
    void pause();
    void resume();
    bool isPlaying();

    private:
    raudio::Sound m_sound;
};
