#pragma once

#include "ScreenPoint.h"

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Spatial::ScreenPoint>
    {
        using Type = Spatial::ScreenPoint;
        using Management = ObjectManagement<Type>;

        static String Name() { return "ScreenPoint"; }
        static String ContainingNamespace() { return "Atmos::Spatial"; }
        static String Documentation() { return "Represents a point in screen coordinates."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };
}