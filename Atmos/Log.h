#pragma once

#include <Arca/Command.h>

#include "LoggingSeverity.h"
#include "LoggingDetails.h"
#include "String.h"

namespace Atmos::Logging
{
    struct Log
    {
        String message;
        Severity severity;
        Details details;

        explicit Log(String message);
        Log(String message, Severity severity);
        Log(String message, const Details& details);
        Log(String message, Severity severity, const Details& details);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Logging::Log>
    {
        static constexpr ObjectType objectType = ObjectType::Command;
        static const inline TypeName typeName = "Atmos::Logging::Log";
    };
}