#pragma once

#include <concepts>

template<typename T>
concept RenderableTrait = requires(T t) { t.render(); };
