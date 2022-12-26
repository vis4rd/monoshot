#include "../../include/audio/AudioManager.hpp"

AudioManager& AudioManager::get()
{
    static AudioManager instance;
    return instance;
}

bool AudioManager::addSound(const std::string& name, const std::filesystem::path& path, const float& volume)
{
    if(this->doesNameExist(name))
    {
        return false;
    }

    sf::SoundBuffer buffer;
    bool success = false;
    if(success = buffer.loadFromFile(path); success)
    {
        m_soundNames[name] = getNextFreeIndex();
        m_soundBuffers.push_back(std::move(buffer));
        m_soundQueues.emplace_back();
        m_soundVolumes.emplace_back(volume);
    }

    return success;
}

void AudioManager::playSound(const std::string& name)
{
    if(not this->doesNameExist(name))
    {
        return;
    }

    const auto& index = m_soundNames.at(name);
    auto& queue = m_soundQueues.at(index);
    const auto& volume = m_soundVolumes.at(index);

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
    new_sound.setVolume(100.f * volume);
    queue.push_back(std::move(new_sound));

    queue.back().play();
}

bool AudioManager::setVolume(const std::string& name, const float& new_volume)
{
    if(not this->doesNameExist(name))
    {
        return false;
    }
    auto volume = std::clamp(new_volume, 0.f, 1.f);

    const auto& index = m_soundNames.at(name);
    m_soundVolumes.at(index) = volume;
    return true;
}

const float& AudioManager::getVolume(const std::string& name) const
{
    const auto& index = m_soundNames.at(name);
    return m_soundVolumes.at(index);
}

std::size_t AudioManager::getNextFreeIndex() const
{
    return m_soundBuffers.size();
}

bool AudioManager::doesNameExist(const std::string& name) const
{
    return m_soundNames.contains(name);
}
