#pragma once

#include "ToWorldPoint3D.h"

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Spatial::ToWorldPoint3D>
    {
        using Type = Spatial::ToWorldPoint3D;
        using Management = ObjectManagement<Type>;

        static String Name() { return "ToWorldPoint3D"; }
        static String ContainingNamespace() { return "Atmos::Spatial"; }
        static String Documentation() { return "This is a command."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        static Spatial::Point3D ToCommandReturn(Arca::command_result_t<Type> fromArca, Arca::Reliquary&);
    };
}