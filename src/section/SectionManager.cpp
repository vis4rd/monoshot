#include "../../include/section/SectionManager.hpp"

SectionManager& SectionManager::get()
{
    static SectionManager instance;
    return instance;
}

Section&& SectionManager::popSection()
{
    auto section = m_sections.top().release();
    m_sections.pop();
    return std::move(*section);
}

size_t SectionManager::size() const noexcept
{
    return m_sections.size();
}

void SectionManager::clear() noexcept
{
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
