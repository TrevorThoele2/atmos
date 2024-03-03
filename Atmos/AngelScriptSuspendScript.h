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

        static inline const String name = "Suspend";
        static inline const String containingNamespace = Namespaces::Atmos::Scripting::name;
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine);
    };
}