#pragma once

#include <Arca/Signal.h>

#include "String.h"

namespace Atmos::Logging
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
    struct Traits<Atmos::Logging::ProcessedLog>
    {
        static const ObjectType objectType = ObjectType::Signal;
        static inline const TypeName typeName = "ProcessedLog";
    };
}