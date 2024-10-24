#pragma once

template<typename T>
concept RenderableTrait = requires(T t) { t.render(); };
