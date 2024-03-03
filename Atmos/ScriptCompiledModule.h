#pragma once

#include "String.h"
#include "Buffer.h"

namespace Atmos::Scripting
{
    struct CompiledModule
    {
        String name;
        Buffer source;
    };
}