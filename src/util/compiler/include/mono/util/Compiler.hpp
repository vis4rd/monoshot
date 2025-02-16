#pragma once

#ifndef __clang_major__
    #define __clang_major__ 0
#endif

#ifndef __GNUC__
    #define __GNUC__ 0
#endif

namespace mono::util
{

consteval bool isGnuCompiler()
{
    return __GNUC__ > 0;
}

consteval bool isClangCompiler()
{
    return __clang_major__ > 0;
}

}  // namespace mono::util
