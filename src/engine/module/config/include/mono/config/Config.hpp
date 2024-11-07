#pragma once

#include <inicpp.h>

#include "ConfigStorage.hpp"
#include "dev_ui/DevUIConfigWindow.hpp"  // IWYU pragma: export
#include "mono/log/priv/BufferedSink.hpp"
#include "priv/StaticConfiguration.hpp"  // IWYU pragma: export
#include "priv/converters/GlmVec2Converter.hpp"  // IWYU pragma: export
#include "priv/converters/SpdlogLevelConverter.hpp"  // IWYU pragma: export

namespace mono::config
{


namespace priv
{

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
inline std::shared_ptr<log::priv::BufferedSink> buffered_sink;

}  // namespace priv

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
inline ConfigStorage runtime;

void initialize();

}  // namespace mono::config
