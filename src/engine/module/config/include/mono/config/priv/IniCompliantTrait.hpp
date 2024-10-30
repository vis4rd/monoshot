#pragma once

#include "inicpp.h"

namespace mono::config
{

template<typename T>
concept IniEncodableTrait = requires(T value, std::string target) {
    { ini::Convert<T>{}.encode(value, target) };
};

template<typename T>
concept IniDecodableTrait = requires(T target, std::string value) {
    { ini::Convert<T>{}.decode(value, target) };
};

template<typename T>
concept IniCompliantTrait = IniEncodableTrait<T> and IniDecodableTrait<T>;

}  // namespace mono::config
