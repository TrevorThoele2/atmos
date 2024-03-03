#pragma once

#include "ModifyProperties.h"

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<ModifyProperties>
    {
        using Type = ModifyProperties;
        using Management = ObjectManagement<Type>;

        static String Name() { return "ModifyProperties"; }
        static String ContainingNamespace() { return "Atmos"; }
        static String Documentation() { return "This is a command."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };
}