#include "../../include/audio/Sound.hpp"

#include "../../res/audio/default_sound.hpp"

Sound::Sound()
{
    raudio::Wave wave = {
        .frameCount = DEFAULT_SOUND_FRAME_COUNT,
        .sampleRate = DEFAULT_SOUND_SAMPLE_RATE,
        .sampleSize = DEFAULT_SOUND_SAMPLE_SIZE,
        .channels = DEFAULT_SOUND_CHANNELS,
        .data = defaultSoundData,
    };

    m_sound = raudio::LoadSoundFromWave(wave);
}

Sound::Sound(const std::string_view& filename)
{
    m_sound = raudio::LoadSound(filename.data());
}

Sound::Sound(const char* filename)
    : Sound(std::string_view(filename))
{
}

Sound::Sound(const Sound& copy)
    : m_sound(copy.m_sound)
{
}

Sound::Sound(Sound&& move)
    : m_sound(std::move(move.m_sound))
{
}

Sound::~Sound()
{
    raudio::UnloadSound(m_sound);
}

Sound& Sound::operator=(const Sound& copy)
{
    m_sound = copy.m_sound;
    return *this;
}

Sound& Sound::operator=(Sound&& move)
{
    m_sound = std::move(move.m_sound);
    return *this;
}

void Sound::setVolume(const float& volume)
{
    raudio::SetSoundVolume(m_sound, volume);
}

void Sound::setPitch(const float& pitch)
{
    raudio::SetSoundPitch(m_sound, pitch);
}

void Sound::setPan(const float& pan)
{
    raudio::SetSoundPan(m_sound, pan);
}

void Sound::play()
{
    raudio::PlaySound(m_sound);
}

void Sound::playMulti()
{
    raudio::PlaySoundMulti(m_sound);
}

void Sound::stop()
{
    raudio::StopSound(m_sound);
}

void Sound::pause()
{
    raudio::PauseSound(m_sound);
}

void Sound::resume()
{
    raudio::ResumeSound(m_sound);
}

bool Sound::isPlaying()
{
    return raudio::IsSoundPlaying(m_sound);
}
