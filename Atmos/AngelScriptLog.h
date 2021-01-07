#pragma once

#include "Log.h"

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

class asIScriptEngine;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Logging::Log>
    {
        using Type = Logging::Log;
        using Management = ObjectManagement<Type>;

        static String Name() { return "Log"; }
        static String ContainingNamespace() { return "Atmos::Logging"; }
        static String Documentation() { return "This is a command."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };
}