#pragma once

#include "Size2D.h"

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Spatial::Size2D>
    {
        using Type = Spatial::Size2D;
        using Management = ObjectManagement<Type>;

        static String Name() { return "Size2D"; }
        static String ContainingNamespace() { return "Atmos::Spatial"; }
        static String Documentation() { return "2D size."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };
}