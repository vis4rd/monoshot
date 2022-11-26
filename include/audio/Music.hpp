#pragma once

#include "AudioObject.hpp"

namespace raudio
{
#include <raudio.h>
}  // namespace raudio

class Music : public AudioObject
{
    public:
    Music();
    Music(const std::string_view& filename);
    Music(const char* filename);
    Music(const Music& copy);
    Music(Music&& move);
    ~Music();

    Music& operator=(const Music& copy);
    Music& operator=(Music&& move);

    void setVolume(const float& volume);
    void setPitch(const float& pitch);
    void setPan(const float& pan);
    void play();
    void stop();
    void pause();
    void resume();
    bool isPlaying() const;
    void seek(const float& position);
    void update();
    float getTimeLength() const;
    float getTimePlayed() const;

    private:
    raudio::Music m_music;
};
