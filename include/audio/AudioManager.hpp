#pragma once

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Music.hpp>

#include <filesystem>

class AudioManager final
{
    public:
    AudioManager(const AudioManager&) = delete;
    AudioManager(AudioManager&&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;
    AudioManager& operator=(AudioManager&&) = delete;
    static AudioManager& get();

    bool addSound(const std::string& name, const std::filesystem::path& path, const float& volume = 1.f);
    void playSound(const std::string& name);
    bool setSoundUserVolume(const std::string& name, const float& new_volume);
    const float& getSoundUserVolume(const std::string& name) const;
    float getSoundVolume(const std::string& name) const;

    bool addMusic(const std::string& name, const std::filesystem::path& path, const float& volume = 1.f);
    void playMusic(const std::string& name);
    void pauseMusic(const std::string& name);
    void stopMusic(const std::string& name);
    const std::unique_ptr<sf::Music>& getMusic(const std::string& name);
    bool setMusicUserVolume(const std::string& name, const float& new_volume);
    const float& getMusicUserVolume(const std::string& name) const;
    float getMusicVolume(const std::string& name) const;

    private:
    AudioManager() = default;
    std::size_t getNextFreeSoundIndex() const;
    std::size_t getNextFreeMusicIndex() const;
    bool doesSoundNameExist(const std::string& name) const;
    bool doesMusicNameExist(const std::string& name) const;

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
};
