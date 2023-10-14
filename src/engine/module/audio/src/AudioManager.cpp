#include "../include/audio/AudioManager.hpp"

static constexpr float baseVolume = 100.f;

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
    return false;
}

void AudioManager::playSound(const std::string& name) { }

bool AudioManager::setSoundUserVolume(const std::string& name, const float& new_volume)
{
    return false;
}

float AudioManager::getSoundUserVolume(const std::string& name) const
{
    return 0.f;
}

float AudioManager::getSoundVolume(const std::string& name) const
{
    return 0.f;
}

bool AudioManager::addMusic(
    const std::string& name,
    const std::filesystem::path& path,
    const float& volume)
{
    return false;
}

void AudioManager::playMusic(const std::string& name) { }

void AudioManager::pauseMusic(const std::string& name) { }

void AudioManager::stopMusic(const std::string& name) { }

bool AudioManager::setMusicUserVolume(const std::string& name, const float& new_volume)
{
    return false;
}

float AudioManager::getMusicUserVolume(const std::string& name) const
{
    return 0.f;
}

float AudioManager::getMusicVolume(const std::string& name) const
{
    return 0.f;
}

void AudioManager::setMixerMasterVolume(const float& new_volume) { }

void AudioManager::setMixerSfxVolume(const float& new_volume) { }

void AudioManager::setMixerMusicVolume(const float& new_volume) { }

float AudioManager::getMixerMasterVolume()
{
    return 0.f;
}

float AudioManager::getMixerSfxVolume()
{
    return 0.f;
}

float AudioManager::getMixerMusicVolume()
{
    return 0.f;
}

std::size_t AudioManager::getNextFreeSoundIndex() const
{
    return 0ULL;
}

std::size_t AudioManager::getNextFreeMusicIndex() const
{
    return 0ULL;
}

bool AudioManager::doesSoundNameExist(const std::string& name) const
{
    return false;
}

bool AudioManager::doesMusicNameExist(const std::string& name) const
{
    return false;
}

void AudioManager::updateSoundVolume(const std::size_t& queue_index, const std::size_t& sound_index)
{ }

void AudioManager::updateSoundQueueVolume(const std::size_t& queue_index) { }

void AudioManager::updateSfxVolumes() { }

void AudioManager::updateMusicVolumes() { }

void AudioManager::updateAllVolumes() { }
