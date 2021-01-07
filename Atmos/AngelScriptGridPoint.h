#pragma once

#include "GridPoint.h"

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Spatial::Grid::Point>
    {
        using Type = Spatial::Grid::Point;
        using Management = ObjectManagement<Type>;

        static String Name() { return "Point"; }
        static String ContainingNamespace() { return "Atmos::Spatial::Grid"; }
        static String Documentation() { return "Represents a point in grid coordinates."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };
}