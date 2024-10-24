#pragma once

template<typename T>
concept UpdateableTrait = requires(T t) { t.update(); };
