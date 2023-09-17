#pragma once

#include <filesystem>
#include <unordered_map>
#include <vector>

#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

class AudioManager final
{
    public:
    AudioManager(const AudioManager&) = delete;
    AudioManager(AudioManager&&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;
    AudioManager& operator=(AudioManager&&) = delete;
    static AudioManager& get();

    bool addSound(
        const std::string& name,
        const std::filesystem::path& path,
        const float& volume = 1.f);
    void playSound(const std::string& name);
    bool setSoundUserVolume(const std::string& name, const float& new_volume);
    const float& getSoundUserVolume(const std::string& name) const;
    float getSoundVolume(const std::string& name) const;

    bool addMusic(
        const std::string& name,
        const std::filesystem::path& path,
        const float& volume = 1.f);
    void playMusic(const std::string& name);
    void pauseMusic(const std::string& name);
    void stopMusic(const std::string& name);
    const std::unique_ptr<sf::Music>& getMusic(const std::string& name);
    bool setMusicUserVolume(const std::string& name, const float& new_volume);
    const float& getMusicUserVolume(const std::string& name) const;
    float getMusicVolume(const std::string& name) const;

    void setMixerMasterVolume(const float& new_volume = 1.f);
    void setMixerSfxVolume(const float& new_volume = 1.f);
    void setMixerMusicVolume(const float& new_volume = 1.f);

    const float& getMixerMasterVolume();
    const float& getMixerSfxVolume();
    const float& getMixerMusicVolume();

    private:
    AudioManager() = default;
    std::size_t getNextFreeSoundIndex() const;
    std::size_t getNextFreeMusicIndex() const;
    bool doesSoundNameExist(const std::string& name) const;
    bool doesMusicNameExist(const std::string& name) const;
    void updateSoundVolume(const std::size_t& queue_index, const std::size_t& sound_index);
    void updateSoundQueueVolume(const std::size_t& queue_index);
    void updateSfxVolumes();
    void updateMusicVolumes();
    void updateAllVolumes();

    private:
    std::unordered_map<std::string, std::size_t> m_soundNames;
    std::vector<sf::SoundBuffer> m_soundBuffers;
    std::vector<std::vector<sf::Sound>> m_soundQueues;
    std::vector<float> m_soundInitialVolumes;
    std::vector<float> m_soundUserVolumes;

    std::unordered_map<std::string, std::size_t> m_musicNames;
    std::vector<std::unique_ptr<sf::Music>> m_musics;
    std::vector<float> m_musicInitialVolumes;
    std::vector<float> m_musicUserVolumes;

    float m_mixerMaster = 1.f;
    float m_mixerSfx = 1.f;
    float m_mixerMusic = 1.f;
};
