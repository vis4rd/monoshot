#pragma once

#include <cstdint>

namespace mono::gl
{

class ElementBuffer
{
    public:
    ElementBuffer() = default;
    ElementBuffer(const std::uint32_t* indices, std::uint32_t count);
    ElementBuffer(const ElementBuffer&) = default;
    ElementBuffer(ElementBuffer&&) = default;
    ~ElementBuffer();

    ElementBuffer& operator=(const ElementBuffer&) = default;
    ElementBuffer& operator=(ElementBuffer&&) = default;

    void bind() const;
    void unbind() const;

    const std::uint32_t& getID() const;
    std::uint32_t getElementCount() const;

    void setData(std::uint32_t* indices, std::uint32_t count);
    bool isInitialized() const;

    // NOLINTNEXTLINE(google-explicit-constructor)
    operator std::uint32_t() const;

    private:
    std::uint32_t m_id{};
    std::uint32_t m_count{};
    bool m_isInit = false;
};

}  // namespace mono::gl
