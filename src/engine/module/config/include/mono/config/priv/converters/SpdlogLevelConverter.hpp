#pragma once

#include <string>

#include <inicpp.h>
#include <spdlog/common.h>

namespace ini
{

template<>
struct Convert<spdlog::level>
{
    void encode(const spdlog::level& value, std::string& target) const
    {
        switch(value)
        {
            case spdlog::level::trace: target = "trace"; break;
            case spdlog::level::debug: target = "debug"; break;
            case spdlog::level::info: target = "info"; break;
            case spdlog::level::warn: target = "warning"; break;
            case spdlog::level::err: target = "error"; break;
            case spdlog::level::critical: target = "critical"; break;
            case spdlog::level::off: target = "off"; break;
            default: target = "off"; break;
        }
    }

    void decode(const std::string& value, spdlog::level& target) const
    {
        if(value.compare("trace") == 0)
        {
            target = spdlog::level::trace;
        }
        else if(value.compare("debug") == 0)
        {
            target = spdlog::level::debug;
        }
        else if(value.compare("info") == 0)
        {
            target = spdlog::level::info;
        }
        else if(value.compare("warning") == 0)
        {
            target = spdlog::level::warn;
        }
        else if(value.compare("error") == 0)
        {
            target = spdlog::level::err;
        }
        else if(value.compare("critical") == 0)
        {
            target = spdlog::level::critical;
        }
        else if(value.compare("off") == 0)
        {
            target = spdlog::level::off;
        }
        else
        {
            target = spdlog::level::off;
        }
    }
};

}  // namespace ini
