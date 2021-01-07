#pragma once

#include "SuspendScript.h"

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Suspend>
    {
        using Type = Suspend;
        using Management = ObjectManagement<Type>;

        static String Name() { return "Suspend"; }
        static String ContainingNamespace() { return "Atmos::Scripting"; }
        static String Documentation() { return "This is a command."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };
}