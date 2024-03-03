#pragma once

#include "IsSolid.h"

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

#include <Arca/CommandResult.h>

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<World::IsSolid>
    {
        using Type = World::IsSolid;
        using Management = ObjectManagement<Type>;

        static String Name() { return "IsSolid"; }
        static String ContainingNamespace() { return "Atmos::World"; }
        static String Documentation() { return "This is a command."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        static bool ToCommandReturn(Arca::command_result_t<Type> fromArca, Arca::Reliquary&);
    };
}