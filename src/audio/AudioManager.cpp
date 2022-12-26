#include "../../include/audio/AudioManager.hpp"

AudioManager& AudioManager::get()
{
    static AudioManager instance;
    return instance;
}

bool AudioManager::addSound(const std::string& name, const std::filesystem::path& path)
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
    queue.push_back(std::move(new_sound));

    queue.back().play();
}

std::size_t AudioManager::getNextFreeIndex() const
{
    return m_soundBuffers.size();
}

bool AudioManager::doesNameExist(const std::string& name) const
{
    return m_soundNames.contains(name);
}
