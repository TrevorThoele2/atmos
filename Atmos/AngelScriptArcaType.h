#pragma once

#include <Arca/Type.h>

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Arca::Type>
    {
        using Type = Arca::Type;
        using Management = ObjectManagement<Type>;

        static String Name() { return "Type"; }
        static String ContainingNamespace() { return "Arca"; }
        static String Documentation() { return "Represents a type registered with Arca at runtime."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        [[nodiscard]] static String Name(Type type);
    };
}