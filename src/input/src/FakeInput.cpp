#include "../include/input/FakeInput.hpp"

#include "../include/input/priv/InputData.hpp"

namespace mono::input
{

void fakePress(std::int32_t key)
{
    const auto index = static_cast<std::size_t>(key);
    data::fakePressedEvents.at(index) = true;
}

}  // namespace mono::input
