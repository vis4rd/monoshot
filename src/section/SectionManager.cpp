#include "../../include/section/SectionManager.hpp"

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
