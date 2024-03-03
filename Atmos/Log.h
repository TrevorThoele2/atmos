#pragma once

#include <Arca/SignalTraits.h>

#include "LogSeverity.h"
#include "LogNameValuePairs.h"
#include "String.h"

#include <optional>

namespace Atmos::Log
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
    struct Traits<Atmos::Log::Log>
    {
        static const ObjectType objectType = ObjectType::Signal;
        static const TypeName typeName;
    };
}