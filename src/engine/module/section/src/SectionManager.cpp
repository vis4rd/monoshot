#include "../include/section/SectionManager.hpp"

#include "audio/AudioManager.hpp"
#include "resource/ResourceManager.hpp"

SectionManager& SectionManager::get()
{
    static SectionManager instance;
    return instance;
}

Section&& SectionManager::releaseSection()
{
    auto section = m_sections.top().release();
    spdlog::info("Releasing {} in SectionManager", section->name());
    m_sections.pop();
    return std::move(*section);
}

void SectionManager::popSection()
{
    m_sections.pop();
}

const Section& SectionManager::topSection() const
{
    return *(m_sections.top().get());
}

std::size_t SectionManager::size() const noexcept
{
    return m_sections.size();
}

void SectionManager::clear() noexcept
{
    spdlog::info("Removing all sections");
    while(!m_sections.empty())
    {
        m_sections.pop();
    }
}

void SectionManager::update() noexcept
{
    if(!m_sections.empty())
    {
        auto& audio = AudioManager::get();
        auto& menu_music = audio.getMusic("menu_music");
        if((m_sections.top()->name() != "DebugSection")
            && (m_sections.top()->name() != "CreatorSection")
            && (m_sections.top()->name() != "TutorialMapSection")
            && (m_sections.top()->name() != "ForestMapSection")
            && (m_sections.top()->name() != "WinterMapSection"))
        {
            if(menu_music->getStatus() != sf::Music::Playing)
            {
                menu_music->play();
            }
        }
        else
        {
            menu_music->pause();
        }
        m_sections.top()->update();
    }
}

void SectionManager::render() noexcept
{
    if(!m_sections.empty())
    {
        m_sections.top()->render();
    }
}

SectionManager::SectionManager()
{
    AudioManager::get().playMusic("menu_music");
}

SectionManager::~SectionManager()
{
    AudioManager::get().pauseMusic("menu_music");
}
