#pragma once

#include "AngelScriptGlobalManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    class GridSpatialAlgorithms
    {};

    template<>
    struct Registration<GridSpatialAlgorithms>
    {
        using Management = GlobalManagement;

        static String Name() { return "SpatialAlgorithms"; }
        static String ContainingNamespace() { return "Atmos::Spatial::Grid"; }
        static String Documentation() { return ""; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };
}