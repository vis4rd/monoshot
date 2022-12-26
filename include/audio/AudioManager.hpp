#pragma once

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

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

    bool setVolume(const std::string& name, const float& new_volume);
    const float& getVolume(const std::string& name) const;

    private:
    AudioManager() = default;
    std::size_t getNextFreeIndex() const;
    bool doesNameExist(const std::string& name) const;

    private:
    std::unordered_map<std::string, std::size_t> m_soundNames;
    std::vector<sf::SoundBuffer> m_soundBuffers;
    std::vector<std::vector<sf::Sound>> m_soundQueues;
    std::vector<float> m_soundVolumes;
};
