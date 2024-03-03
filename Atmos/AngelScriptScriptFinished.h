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
        static inline const String containingNamespace = Namespaces::Atmos::Scripting::name;
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine);
    };
}