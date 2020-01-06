#pragma once

#include <Arca/Signal.h>

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
        Log(String message, Severity severity);
        Log(String message, Severity severity, const Details& details);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Logging::Log>
    {
        static const ObjectType objectType = ObjectType::Signal;
        static inline const TypeName typeName = "Log";
    };
}