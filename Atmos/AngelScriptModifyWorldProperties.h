#pragma once

#include "ModifyWorldProperties.h"

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<World::ModifyProperties>
    {
        using Type = World::ModifyProperties;
        using Management = ObjectManagement<Type>;

        static String Name() { return "ModifyProperties"; }
        static String ContainingNamespace() { return "Atmos::World"; }
        static String Documentation() { return "This is a command."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };
}