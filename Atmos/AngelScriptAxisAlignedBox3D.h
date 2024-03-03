#pragma once

#include "AxisAlignedBox3D.h"

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Spatial::AxisAlignedBox3D>
    {
        using Type = Spatial::AxisAlignedBox3D;
        using Management = ObjectManagement<Type>;

        static String Name() { return "AxisAlignedBox3D"; }
        static String ContainingNamespace() { return "Atmos::Spatial"; }
        static String Documentation() { return "3D axis aligned bounding box."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };
}