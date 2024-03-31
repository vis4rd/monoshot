#pragma once

class PerspectiveCamera;

namespace mono::gl
{

class RenderWindow;

struct RenderWindowUserStorage
{
    RenderWindow& window;  // reference to self has to be guaranteed
    PerspectiveCamera* camera = nullptr;
};

}  // namespace mono::gl

struct GLFWwindow;

namespace mono
{

void glfwSetWindowUserPointer(GLFWwindow* window, mono::gl::RenderWindowUserStorage& pointer);
mono::gl::RenderWindowUserStorage& glfwGetWindowUserPointer(GLFWwindow* window);

}  // namespace mono
