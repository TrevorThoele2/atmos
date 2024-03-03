#pragma once

#include "LoggingSeverity.h"

#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Logging::Severity>
    {
        static inline const String name = "Severity";
        static inline const String containingNamespace = "Atmos::Logging";
        static inline const String documentation = "Represents the severity of a particular log.";
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };
}