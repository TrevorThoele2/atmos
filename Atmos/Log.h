#pragma once

#include <Arca/Signal.h>

#include "LogSeverity.h"
#include "LogNameValuePairs.h"
#include "String.h"

#include <optional>

namespace Atmos::Logging
{
    class Log
    {
    public:
        const String message;
        const Severity severity;
        const std::optional<NameValuePairs> nameValuePairs;
    public:
        Log(String message, Severity severity);
        Log(String message, Severity severity, const NameValuePairs& nameValuePairs);
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