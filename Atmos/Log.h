#pragma once

#include <Arca/Command.h>

#include "LoggingSeverity.h"
#include "LoggingDetails.h"
#include "String.h"

#include <optional>

namespace Atmos::Logging
{
    class Log
    {
    public:
        const String message;
        const Severity severity;
        const std::optional<Details> details;
    public:
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
        static const ObjectType objectType = ObjectType::Command;
        static inline const TypeName typeName = "Atmos::Logging::Log";
    };
}