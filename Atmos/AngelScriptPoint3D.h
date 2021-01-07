#pragma once

#include "Point3D.h"

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Spatial::Point3D>
    {
        using Type = Spatial::Point3D;
        using Management = ObjectManagement<Type>;

        static String Name() { return "Point3D"; }
        static String ContainingNamespace() { return "Atmos::Spatial"; }
        static String Documentation() { return "3D point."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };
}