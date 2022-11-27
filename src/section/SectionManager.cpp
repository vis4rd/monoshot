#include "../../include/section/SectionManager.hpp"

#include "../../include/utility/ResourceManager.hpp"

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
    // UpdateMusicStream(m_music);
    if(!m_sections.empty())
    {
        if((m_sections.top()->name() != "DebugSection") && (m_sections.top()->name() != "CreatorSection"))
        {
            if(m_menuMusic.getStatus() != sf::Music::Playing)
            {
                m_menuMusic.play();
            }
        }
        else
        {
            m_menuMusic.pause();
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
    if(bool success = m_menuMusic.openFromFile("../res/audio/music/Fragments_ambient.mp3"); not success)
    {
        spdlog::debug("Could not load music from file 'res/audio/music/Fragments_ambient.mp3'");
        throw std::runtime_error("Could not load music from file 'res/audio/music/Fragments_ambient.mp3'");
    }
    m_menuMusic.play();
}

SectionManager::~SectionManager() { }
