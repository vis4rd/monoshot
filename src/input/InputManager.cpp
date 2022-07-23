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
        [&group](const KeyGroup& g)
        {
            return (g.name == group);
        });
    if(iter != m_keybinds.end())
    {
        spdlog::debug("Processing group '{}', keybind states:", group);
        for(auto& [glfw_key, keybind] : iter->keybinds)
        {
            auto& rs = keybind.required_state;
            auto& ps = keybind.previous_state;
            auto& cs = keybind.current_state;
            auto glfw_state = glfwGetKey(window, keybind.key);

            switch(glfw_state)
            {
                case GLFW_PRESS:
                {
                    if(ps == KeyState::IDLE || ps == KeyState::RELEASE)
                    {
                        cs = KeyState::PRESS_ONCE;
                    }
                    else if(ps == KeyState::PRESS_ONCE)
                    {
                        cs = KeyState::HOLD;
                    }
                    // if hold: hold
                    // if repeat: repeat
                    break;
                }
                case GLFW_RELEASE:
                {
                    if(ps == KeyState::REPEAT || ps == KeyState::HOLD || ps == KeyState::PRESS_ONCE)
                    {
                        cs = KeyState::RELEASE;
                    }
                    else if(ps == KeyState::RELEASE)
                    {
                        cs = KeyState::IDLE;
                    }
                    // if idle: idle
                    break;
                }
                case GLFW_REPEAT:  // currently unused
                {
                    spdlog::warn("Using GLFW_REPEAT in InputManager, which is currently unsupported");
                    break;
                }
                default:
                {
                    spdlog::warn("Using unknown key action");
                    break;
                }
            }


            spdlog::debug("Key {}: [{}] -> [{}] (r: [{}])", keybind.key, ps, cs, rs);

            if(cs == rs)
            {
                keybind.callback();
            }
            ps = cs;
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
        [&group](const KeyGroup& g)
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
        [&group](const KeyGroup& g)
        {
            return (g.name == group);
        });
    if(iter != m_keybinds.end())
    {
        iter->keybinds.erase(glfw_key);
    }
}
