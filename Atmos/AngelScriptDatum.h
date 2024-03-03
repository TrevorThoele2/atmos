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

        static String Name() { return "Datum"; }
        static String ContainingNamespace() { return "Atmos"; }
        static String Documentation() { return "Generic key value pair."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };
}