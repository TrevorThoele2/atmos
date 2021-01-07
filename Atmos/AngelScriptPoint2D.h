#pragma once

#include "Point2D.h"

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Spatial::Point2D>
    {
        using Type = Spatial::Point2D;
        using Management = ObjectManagement<Type>;

        static String Name() { return "Point2D"; }
        static String ContainingNamespace() { return "Atmos::Spatial"; }
        static String Documentation() { return "2D point."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };
}