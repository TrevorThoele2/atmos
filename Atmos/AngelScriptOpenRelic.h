#pragma once

#include <Arca/LocalRelic.h>

#include "AngelScriptIndex.h"
#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Arca::OpenRelic>
    {
        using Type = Arca::Index<Arca::OpenRelic>;
        using Management = ObjectManagement<Type>;

        static String Name() { return "OpenRelic"; }
        static String ContainingNamespace() { return "Arca"; }
        static String Documentation() { return "This is a relic. Contains a Bounds."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };

    template<>
    struct Registration<Arca::Index<Arca::OpenRelic>> : Registration<Arca::OpenRelic>
    {};
}