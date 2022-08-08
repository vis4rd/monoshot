#pragma once

#include <cstdint>

class ElementBuffer
{
    public:
    ElementBuffer(std::uint32_t* indices, std::uint32_t count);
    ~ElementBuffer();

    void bind() const;
    void unbind() const;

    std::uint32_t getElementCount() const;

    operator std::uint32_t() const;

    private:
    std::uint32_t m_id;
    std::uint32_t m_count;
};