#include "../../include/audio/AudioManager.hpp"

#include <spdlog/spdlog.h>

AudioManager& AudioManager::get()
{
    static AudioManager instance;
    return instance;
}

bool AudioManager::addSound(const std::string& name, const std::filesystem::path& path, const float& volume)
{
    if(this->doesSoundNameExist(name))
    {
        return false;
    }

    sf::SoundBuffer buffer;
    bool success = false;
    if(success = buffer.loadFromFile(path); success)
    {
        m_soundNames[name] = getNextFreeSoundIndex();
        m_soundBuffers.push_back(std::move(buffer));
        m_soundQueues.emplace_back();
        m_soundInitialVolumes.emplace_back(volume);
        m_soundUserVolumes.emplace_back(1.f);
    }

    return success;
}

void AudioManager::playSound(const std::string& name)
{
    if(not this->doesSoundNameExist(name))
    {
        return;
    }

    const auto& index = m_soundNames.at(name);
    auto& queue = m_soundQueues.at(index);
    const auto& init_volume = m_soundInitialVolumes.at(index);
    const auto& user_volume = m_soundUserVolumes.at(index);

    auto iter = queue.begin();
    while(iter != queue.end())
    {
        if(iter->getStatus() == sf::Sound::Stopped)
        {
            iter->play();
            return;
        }
        iter++;
    }

    // all sounds are still playing, but we need another one
    sf::Sound new_sound;
    new_sound.setBuffer(m_soundBuffers.at(index));
    new_sound.setVolume(100.f * init_volume * user_volume);
    queue.push_back(std::move(new_sound));

    queue.back().play();
}

bool AudioManager::setSoundUserVolume(const std::string& name, const float& new_volume)
{
    if(not this->doesSoundNameExist(name))
    {
        return false;
    }
    auto new_user_volume = std::clamp(new_volume, 0.f, 1.f);

    const auto& index = m_soundNames.at(name);
    m_soundUserVolumes.at(index) = new_user_volume;

    // update all existing sounds
    const auto& init_volume = m_soundInitialVolumes.at(index);
    for(const auto volume = 100.f * init_volume * new_user_volume; auto& sound : m_soundQueues[index])
    {
        sound.setVolume(volume);
    }

    return true;
}

const float& AudioManager::getSoundUserVolume(const std::string& name) const
{
    const auto& index = m_soundNames.at(name);
    return m_soundUserVolumes.at(index);
}

float AudioManager::getSoundVolume(const std::string& name) const
{
    const auto& index = m_soundNames.at(name);
    const auto& init_volume = m_soundInitialVolumes.at(index);
    const auto& user_volume = m_soundUserVolumes.at(index);
    return 100.f * init_volume * user_volume;
}

bool AudioManager::addMusic(const std::string& name, const std::filesystem::path& path, const float& volume)
{
    if(this->doesMusicNameExist(name))
    {
        return false;
    }

    auto music = std::make_unique<sf::Music>();
    bool success = false;
    if(success = music->openFromFile(path); success)
    {
        m_musicNames[name] = getNextFreeMusicIndex();
        auto& init_volume = m_musicInitialVolumes.emplace_back(volume);
        auto& user_volume = m_musicUserVolumes.emplace_back(1.f);
        music->setVolume(100.f * init_volume * user_volume);
        m_musics.push_back(std::move(music));
    }

    return success;
}

void AudioManager::playMusic(const std::string& name)
{
    if(not this->doesMusicNameExist(name))
    {
        return;
    }

    const auto& index = m_musicNames.at(name);
    m_musics.at(index)->play();
}

void AudioManager::pauseMusic(const std::string& name)
{
    if(not this->doesMusicNameExist(name))
    {
        return;
    }

    const auto& index = m_musicNames.at(name);
    m_musics.at(index)->pause();
}

void AudioManager::stopMusic(const std::string& name)
{
    if(not this->doesMusicNameExist(name))
    {
        return;
    }

    const auto& index = m_musicNames.at(name);
    m_musics.at(index)->stop();
}

const std::unique_ptr<sf::Music>& AudioManager::getMusic(const std::string& name)
{
    const auto& index = m_musicNames.at(name);
    return m_musics.at(index);
}

bool AudioManager::setMusicUserVolume(const std::string& name, const float& new_volume)
{
    if(not this->doesMusicNameExist(name))
    {
        return false;
    }
    auto new_user_volume = std::clamp(new_volume, 0.f, 1.f);

    const auto& index = m_musicNames.at(name);
    m_musicUserVolumes.at(index) = new_user_volume;
    const auto& init_volume = m_musicInitialVolumes.at(index);
    m_musics.at(index)->setVolume(100.f * init_volume * new_user_volume);

    return true;
}

const float& AudioManager::getMusicUserVolume(const std::string& name) const
{
    const auto& index = m_musicNames.at(name);
    return m_musicUserVolumes.at(index);
}

float AudioManager::getMusicVolume(const std::string& name) const
{
    const auto& index = m_musicNames.at(name);
    const auto& init_volume = m_musicInitialVolumes.at(index);
    const auto& user_volume = m_musicUserVolumes.at(index);
    return 100.f * init_volume * user_volume;
}

std::size_t AudioManager::getNextFreeSoundIndex() const
{
    return m_soundBuffers.size();
}

std::size_t AudioManager::getNextFreeMusicIndex() const
{
    return m_musics.size();
}

bool AudioManager::doesSoundNameExist(const std::string& name) const
{
    return m_soundNames.contains(name);
}

bool AudioManager::doesMusicNameExist(const std::string& name) const
{
    return m_musicNames.contains(name);
}
