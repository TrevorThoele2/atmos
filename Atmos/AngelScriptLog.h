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

        static inline const String name = "Log";
        static inline const String containingNamespace = Namespaces::Atmos::Logging::name;
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine);
    };
}