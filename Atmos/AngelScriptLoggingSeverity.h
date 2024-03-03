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
        static String Name() { return "Severity"; }
        static String ContainingNamespace() { return "Atmos::Logging"; }
        static String Documentation() { return "Represents the severity of a particular log."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };
}