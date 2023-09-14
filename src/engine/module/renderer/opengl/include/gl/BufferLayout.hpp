#pragma once

#include <cstdint>
#include <initializer_list>
#include <vector>

#include "BufferElement.hpp"

namespace mono
{

class BufferLayout
{
    public:
    BufferLayout() = default;
    BufferLayout(std::initializer_list<BufferElement> elements);

    std::uint32_t getStride() const;
    const std::vector<BufferElement>& getElements() const;

    std::vector<BufferElement>::iterator begin();
    std::vector<BufferElement>::iterator end();
    std::vector<BufferElement>::const_iterator begin() const;
    std::vector<BufferElement>::const_iterator end() const;

    private:
    void calculateOffsetAndStride();

    private:
    std::vector<BufferElement> m_elements;
    std::uint32_t m_stride = 0u;
};


}  // namespace mono
