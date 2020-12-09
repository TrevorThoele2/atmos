#pragma once

#include "Datum.h"

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Datum>
    {
        using Type = Datum;
        using Management = ObjectManagement<Type>;

        static inline const String name = "Datum";
        static inline const String containingNamespace = Namespaces::Atmos::name;
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine);
    };
}