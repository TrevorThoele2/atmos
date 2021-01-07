#pragma once

#include "Scalers2D.h"

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Spatial::Scalers2D>
    {
        using Type = Spatial::Scalers2D;
        using Management = ObjectManagement<Type>;

        static String Name() { return "Scalers2D"; }
        static String ContainingNamespace() { return "Atmos::Spatial"; }
        static String Documentation() { return "2D scalers."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };
}