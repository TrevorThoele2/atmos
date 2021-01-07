#pragma once

#include "AngelScriptGlobalManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    class SpatialAlgorithms
    {};

    template<>
    struct Registration<SpatialAlgorithms>
    {
        using Management = GlobalManagement;

        static String Name() { return "SpatialAlgorithms"; }
        static String ContainingNamespace() { return "Atmos::Spatial"; }
        static String Documentation() { return ""; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };
}