#pragma once

#include <string>
#include <unordered_map>

namespace mono::dev_ui::priv::state
{

// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
inline std::unordered_map<std::string, bool> window_visibility_flags{};
// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

}  // namespace mono::dev_ui::priv::state
