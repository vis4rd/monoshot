#include "../include/audio/AudioManager.hpp"

#include <spdlog/spdlog.h>

static constexpr float base_volume = 100.f;

AudioManager& AudioManager::get()
{
    static AudioManager instance;
    return instance;
}

bool AudioManager::addSound(
    const std::string& name,
    const std::filesystem::path& path,
    const float& volume)
{
    if(this->doesSoundNameExist(name))
    {
        return false;
    }

    sf::SoundBuffer buffer;
    bool success = false;
    if(success = buffer.loadFromFile(path.string()); success)
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
    this->updateSoundVolume(index, queue.size() - 1);

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
    this->updateSoundQueueVolume(index);

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
    const auto& queue = m_soundQueues.at(index);
    float retval = 1.f;
    if(queue.size() > 0)
    {
        retval = queue.at(0).getVolume();
    }
    return retval;
}

bool AudioManager::addMusic(
    const std::string& name,
    const std::filesystem::path& path,
    const float& volume)
{
    if(this->doesMusicNameExist(name))
    {
        return false;
    }

    auto music = std::make_unique<sf::Music>();
    bool success = false;
    if(success = music->openFromFile(path.string()); success)
    {
        m_musicNames[name] = getNextFreeMusicIndex();
        auto& init_volume = m_musicInitialVolumes.emplace_back(volume);
        auto& user_volume = m_musicUserVolumes.emplace_back(1.f);
        music->setVolume(base_volume * init_volume * user_volume * m_mixerMaster * m_mixerMusic);
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
    m_musics.at(index)->setVolume(
        base_volume * init_volume * new_user_volume * m_mixerMaster * m_mixerMusic);

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
    return base_volume * init_volume * user_volume * m_mixerMaster * m_mixerMusic;
}

void AudioManager::setMixerMasterVolume(const float& new_volume)
{
    m_mixerMaster = std::clamp(new_volume, 0.f, 1.f);
    this->updateAllVolumes();
}

void AudioManager::setMixerSfxVolume(const float& new_volume)
{
    m_mixerSfx = std::clamp(new_volume, 0.f, 1.f);
    this->updateSfxVolumes();
}

void AudioManager::setMixerMusicVolume(const float& new_volume)
{
    m_mixerMusic = std::clamp(new_volume, 0.f, 1.f);
    this->updateMusicVolumes();
}

const float& AudioManager::getMixerMasterVolume()
{
    return m_mixerMaster;
}

const float& AudioManager::getMixerSfxVolume()
{
    return m_mixerSfx;
}

const float& AudioManager::getMixerMusicVolume()
{
    return m_mixerMusic;
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

void AudioManager::updateSoundVolume(const std::size_t& queue_index, const std::size_t& sound_index)
{
    // spdlog::trace("    Updating SFX sound {} in queue {}:", sound_index, queue_index);
    const auto& init_volume = m_soundInitialVolumes.at(queue_index);
    const auto& user_volume = m_soundUserVolumes.at(queue_index);
    m_soundQueues.at(queue_index)
        .at(sound_index)
        .setVolume(base_volume * m_mixerMaster * m_mixerSfx * init_volume * user_volume);
}

void AudioManager::updateSoundQueueVolume(const std::size_t& queue_index)
{
    // spdlog::trace("  Updating SFX queue {}:", queue_index);
    for(std::size_t sound_index{0}; sound_index < m_soundQueues.at(queue_index).size();
        sound_index++)
    {
        this->updateSoundVolume(queue_index, sound_index);
    }
}

void AudioManager::updateSfxVolumes()
{
    // spdlog::trace("Updating all SFX volumes:");
    for(const auto& [name, index] : m_soundNames)
    {
        this->updateSoundQueueVolume(index);
    }
}

void AudioManager::updateMusicVolumes()
{
    for(const auto& [name, index] : m_musicNames)
    {
        const auto& music = m_musics.at(index);
        const auto& init_volume = m_musicInitialVolumes.at(index);
        const auto& user_volume = m_musicUserVolumes.at(index);
        music->setVolume(base_volume * init_volume * user_volume * m_mixerMaster * m_mixerMusic);
    }
}

void AudioManager::updateAllVolumes()
{
    this->updateSfxVolumes();
    this->updateMusicVolumes();
}
