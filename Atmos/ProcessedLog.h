#pragma once

#include <Arca/Signal.h>

#include "String.h"
#include "LoggingSeverity.h"
#include "LoggingDetails.h"

namespace Atmos::Logging
{
    class ProcessedLog
    {
    public:
        const String resultantMessage;

        const String originalMessage;
        const Severity severity;
        const std::optional<Details> details;
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