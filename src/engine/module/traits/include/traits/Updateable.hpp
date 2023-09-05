#pragma once

#include <concepts>

template<typename T>
concept UpdateableTrait = requires(T t) { t.update(); };
