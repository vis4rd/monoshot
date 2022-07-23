#include "../../include/section/NewGameSection.hpp"

NewGameSection::NewGameSection() : Section()
{
    m_name = "NewGameSection";
    auto& input_manager = InputManager::get();
    auto group_id = input_manager.addGroup(m_name);
    input_manager.addKeybind(group_id,
        GLFW_KEY_ESCAPE,
        KeyState::PRESS_ONCE,
        []
        {
            SectionManager::get().clear();
        });
}

void NewGameSection::update() noexcept { }

void NewGameSection::render() noexcept { }
