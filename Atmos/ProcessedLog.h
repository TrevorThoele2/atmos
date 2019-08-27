#pragma once

#include <Arca/SignalTraits.h>

#include "String.h"

namespace Atmos::Log
{
    class ProcessedLog
    {
    public:
        String string;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Log::ProcessedLog>
    {
        static const ObjectType objectType = ObjectType::Signal;
        static const TypeName typeName;
    };
}