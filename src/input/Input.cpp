#include "../../include/input/Input.hpp"

const std::size_t Input::addGroup(const std::string& name)
{
    Group new_group;
    if(name.empty())
    {
        new_group.name = "Keybind Group No. " + std::to_string(m_keybinds.size());
    }
    else
    {
        new_group.name = name;
    }
    m_keybinds.push_back(new_group);
    // std::cout << m_keybinds[0].name << std::endl;
    return m_keybinds.size() - 1;
}

void Input::processGroup(GLFWwindow* window, const std::string& group)
{
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
            keybind.callback(window);
        }
    }
    else
    {
        throw std::runtime_error("Given keybind group does not exist.");
    }
}

void Input::removeGroup(const std::string& group)
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

void Input::removeKeybind(const std::string& group, int32_t glfw_key)
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
