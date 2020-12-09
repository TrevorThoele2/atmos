#pragma once

#include "LoggingSeverity.h"

#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;

namespace Atmos::Scripting::Angel
{
    class LoggingSeverity
    {};

    template<>
    struct Registration<LoggingSeverity>
    {
        static inline const String name = "Severity";
        static inline const String containingNamespace = Namespaces::Atmos::Logging::name;
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine);
    };
}