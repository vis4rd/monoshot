#include "../../include/input/InputManager.hpp"

InputManager& InputManager::get()
{
    static InputManager instance;
    return instance;
}

const std::size_t InputManager::addGroup(const std::string& name)
{
    auto valid_name(name);
    if(valid_name.empty())
    {
        valid_name = "Keybind Group No. " + std::to_string(m_keybinds.size());
    }
    m_keybinds.emplace_back(std::move(valid_name));
    spdlog::info("Creating a new keybind group called '{}'", m_keybinds[m_keybinds.size() - 1].name);
    return m_keybinds.size() - 1;
}

void InputManager::processGroup(GLFWwindow* window, const std::string& group)
{
    spdlog::trace("Processing keybind group '{}'", group);
    auto iter = std::find_if(m_keybinds.begin(),
        m_keybinds.end(),
        [&group](const Group& g)
        {
            return (g.name == group);
        });
    if(iter != m_keybinds.end())
    {
        for(auto& [glfw_key, keybind] : iter->keybinds)
        {
            keybind.callback(window, keybind.key, keybind.action);
        }
    }
    else
    {
        throw std::runtime_error("Given keybind group does not exist.");
    }
}

void InputManager::removeGroup(const std::string& group)
{
    auto iter = std::find_if(m_keybinds.begin(),
        m_keybinds.end(),
        [&group](const Group& g)
        {
            return (g.name == group);
        });
    if(iter != m_keybinds.end())
    {
        iter->keybinds.clear();
        std::swap(*iter, m_keybinds.back());
        m_keybinds.pop_back();
    }
}

void InputManager::removeKeybind(const std::string& group, int32_t glfw_key)
{
    auto iter = std::find_if(m_keybinds.begin(),
        m_keybinds.end(),
        [&group](const Group& g)
        {
            return (g.name == group);
        });
    if(iter != m_keybinds.end())
    {
        iter->keybinds.erase(glfw_key);
    }
}
