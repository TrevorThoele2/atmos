#pragma once

#include "GridSize.h"

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Spatial::Grid::Size>
    {
        using Type = Spatial::Grid::Size;
        using Management = ObjectManagement<Type>;

        static String Name() { return "Size"; }
        static String ContainingNamespace() { return "Atmos::Spatial::Grid"; }
        static String Documentation() { return "Grid size."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };
}