#pragma once

#ifndef DEBUG_BUILD
    #define DEBUG_BUILD 0
#endif

namespace mono::config::constant
{
constexpr bool debugBuild = DEBUG_BUILD;
}
