#pragma once

#include <string>

class Section
{
    public:
    Section() = default;
    Section(const Section& copy) = default;
    Section(Section&& move) noexcept = default;
    virtual ~Section() = default;

    Section& operator=(const Section& copy) = default;
    Section& operator=(Section&& move) noexcept = default;

    const std::string& name() const noexcept;
    virtual void update() noexcept = 0;
    virtual void render() noexcept = 0;

    protected:
    std::string m_name = "Section";
};

template<typename T>
concept IsSectionTrait = std::derived_from<std::remove_cvref_t<T>, Section>;
