#pragma once

#include <inicpp.h>

#include "ConfigStorage.hpp"
#include "mono/log/priv/BufferedSink.hpp"
#include "priv/StaticConfiguration.hpp"  // IWYU pragma: export
#include "priv/converters/GlmVecConverter.hpp"  // IWYU pragma: export
#include "priv/converters/SpdlogLevelConverter.hpp"  // IWYU pragma: export

namespace mono::config
{


namespace priv
{

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
inline std::shared_ptr<log::priv::BufferedSink> buffered_sink;

}  // namespace priv

// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)

// TODO: docs
inline ConfigStorage runtime;
// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

// TODO: docs
void initialize();

// TODO: docs
void initialize(const std::filesystem::path& path);

}  // namespace mono::config
