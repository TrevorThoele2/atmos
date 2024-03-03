#pragma once

#include "GridAxisAlignedBox.h"

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Spatial::Grid::AxisAlignedBox>
    {
        using Type = Spatial::Grid::AxisAlignedBox;
        using Management = ObjectManagement<Type>;

        static String Name() { return "AxisAlignedBox"; }
        static String ContainingNamespace() { return "Atmos::Spatial::Grid"; }
        static String Documentation() { return "Grid axis aligned bounding box."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };
}