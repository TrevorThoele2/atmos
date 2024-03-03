#pragma once

#include "Angle2D.h"

#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;

namespace Atmos::Scripting::Angel
{
    class Angle2D
    {};

    template<>
    struct Registration<Angle2D>
    {
        static String Name() { return "Angle2D"; }
        static String ContainingNamespace() { return "Atmos::Spatial"; }
        static String Documentation() { return "2D angle."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };
}