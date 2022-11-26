#include "../../include/audio/Music.hpp"

#include "../../res/audio/default_sound.hpp"

Music::Music()
{
    m_music = raudio::LoadMusicStreamFromMemory(".mp3", reinterpret_cast<const unsigned char*>(defaultSoundData), sizeof(defaultSoundData));
}

Music::Music(const std::string_view& filename)
{
    m_music = raudio::LoadMusicStream(filename.data());
}

Music::Music(const char* filename)
    : Music(std::string_view(filename))
{
}

Music::Music(const Music& copy)
    : m_music(copy.m_music)
{
}

Music::Music(Music&& move)
    : m_music(std::move(move.m_music))
{
}

Music::~Music()
{
    raudio::UnloadMusicStream(m_music);
}

Music& Music::operator=(const Music& copy)
{
    m_music = copy.m_music;
    return *this;
}

Music& Music::operator=(Music&& move)
{
    m_music = std::move(move.m_music);
    return *this;
}

void Music::setVolume(const float& volume)
{
    raudio::SetMusicVolume(m_music, volume);
}

void Music::setPitch(const float& pitch)
{
    raudio::SetMusicPitch(m_music, pitch);
}

void Music::setPan(const float& pan)
{
    raudio::SetMusicPan(m_music, pan);
}

void Music::play()
{
    raudio::PlayMusicStream(m_music);
}

void Music::stop()
{
    raudio::StopMusicStream(m_music);
}

void Music::pause()
{
    raudio::PauseMusicStream(m_music);
}

void Music::resume()
{
    raudio::ResumeMusicStream(m_music);
}

bool Music::isPlaying() const
{
    return raudio::IsMusicStreamPlaying(m_music);
}

void Music::seek(const float& position)
{
    raudio::SeekMusicStream(m_music, position);
}

void Music::update()
{
    raudio::UpdateMusicStream(m_music);
}

float Music::getTimeLength() const
{
    return raudio::GetMusicTimeLength(m_music);
}

float Music::getTimePlayed() const
{
    return raudio::GetMusicTimePlayed(m_music);
}
