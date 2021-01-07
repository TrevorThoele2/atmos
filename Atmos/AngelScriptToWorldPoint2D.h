#pragma once

#include "ToWorldPoint2D.h"

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Spatial::ToWorldPoint2D>
    {
        using Type = Spatial::ToWorldPoint2D;
        using Management = ObjectManagement<Type>;

        static String Name() { return "ToWorldPoint2D"; }
        static String ContainingNamespace() { return "Atmos::Spatial"; }
        static String Documentation() { return "This is a command."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        static Spatial::Point2D ToCommandReturn(Arca::command_result_t<Type> fromArca, Arca::Reliquary&);
    };
}