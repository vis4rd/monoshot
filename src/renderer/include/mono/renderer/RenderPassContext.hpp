#pragma once

#include "RenderTarget.hpp"

namespace mono::renderer
{

struct RenderPassContext
{
    RenderTarget* prevOutput = nullptr;  // output of the preceding pass
};

}  // namespace mono::renderer
