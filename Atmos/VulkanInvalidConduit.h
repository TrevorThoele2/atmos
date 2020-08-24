#pragma once

#include "Exception.h"

namespace Atmos::Render::Vulkan
{
    class InvalidConduit final : public Exception
    {
    public:
        InvalidConduit();
    };
}