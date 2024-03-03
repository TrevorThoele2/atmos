#pragma once

#include <Arca/Signal.h>

#include "String.h"
#include "LoggingSeverity.h"
#include "LoggingDetails.h"

namespace Atmos::Logging
{
    struct ProcessedLog
    {
        String resultantMessage;

        String originalMessage;
        Severity severity = Severity::Information;
        std::optional<Details> details;

        ProcessedLog() = default;
        ProcessedLog(
            String resultantMessage, String originalMessage, Severity severity, std::optional<Details> details);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Logging::ProcessedLog>
    {
        static const ObjectType objectType = ObjectType::Signal;
        static TypeName TypeName() { return "Atmos::Logging::ProcessedLog"; }
    };
}