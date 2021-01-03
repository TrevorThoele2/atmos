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

        static inline const String name = "SpatialAlgorithms";
        static inline const String containingNamespace = "Atmos::Spatial::Grid";
        static inline const String documentation = "";
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };
}