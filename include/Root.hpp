#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <filesystem>
#include <fstream>
#include <functional>
#include <map>
#include <memory>
#include <numeric>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <unordered_map>

#ifndef DEBUG_MODE
    #define DEBUG_MODE 0
#endif

namespace Flag
{

constexpr bool DebugMode = DEBUG_MODE;

}
