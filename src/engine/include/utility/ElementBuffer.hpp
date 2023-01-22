#pragma once

#include <cstdint>

class ElementBuffer
{
    public:
    ElementBuffer() = default;
    ElementBuffer(const std::uint32_t* indices, std::uint32_t count);
    ~ElementBuffer();

    void bind() const;
    void unbind() const;

    const std::uint32_t& getID() const;
    std::uint32_t getElementCount() const;

    void setData(std::uint32_t* indices, std::uint32_t count);
    bool isInitialized() const;

    operator std::uint32_t() const;

    private:
    std::uint32_t m_id;
    std::uint32_t m_count;
    bool m_isInit = false;
};
