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

        static inline const String name = "Finished";
        static inline const String containingNamespace = "Atmos::Scripting";
        static inline const String documentation = "This is a signal.";
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };
}