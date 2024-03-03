#pragma once

#include "ScriptFinished.h"

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Finished>
    {
        using Type = Finished;
        using Management = ObjectManagement<Type>;

        static String Name() { return "Finished"; }
        static String ContainingNamespace() { return "Atmos::Scripting"; }
        static String Documentation() { return "This is a signal."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };
}