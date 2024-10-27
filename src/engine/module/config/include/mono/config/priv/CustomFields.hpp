#pragma once

#include <iostream>

#include <inicpp.h>

namespace mono::config::type
{

enum class WindowMode : std::uint8_t
{
    WINDOWED,
    FULLSCREEN,
    BORDERLESS,
};

struct WindowResolution
{
    int width;
    int height;
};

}  // namespace mono::config::type

namespace ini
{

using mono::config::type::WindowMode;
using mono::config::type::WindowResolution;

template<>
struct Convert<WindowMode>
{
    void decode(const std::string &value, WindowMode &result)
    {
        if(value.starts_with("windowed"))
        {
            result = WindowMode::WINDOWED;
        }
        else if(value.starts_with("fullscreen"))
        {
            result = WindowMode::FULLSCREEN;
        }
        else if(value.starts_with("borderless"))
        {
            result = WindowMode::BORDERLESS;
        }
        else
        {
            throw std::invalid_argument("Invalid window mode value: " + value);
        }
    }

    void encode(const WindowMode &value, std::string &result)
    {
        switch(value)
        {
            case WindowMode::WINDOWED: result = "windowed"; break;
            case WindowMode::FULLSCREEN: result = "fullscreen"; break;
            case WindowMode::BORDERLESS: result = "borderless"; break;
        }
    }
};

template<>
struct Convert<WindowResolution>
{
    void decode(const std::string &value, WindowResolution &result)
    {
        const std::string width = value.substr(0, value.find('x'));
        const std::string height = value.substr(value.find('x') + 1);

        Convert<int> int_converter{};
        int_converter.decode(width, result.width);
        int_converter.decode(height, result.height);
    }

    void encode(const WindowResolution &value, std::string &result)
    {
        result.clear();

        std::string encoded;
        Convert<int> int_converter{};
        int_converter.encode(value.width, encoded);
        result += encoded + 'x';
        int_converter.encode(value.height, encoded);
        result += encoded;
    }
};

}  // namespace ini
