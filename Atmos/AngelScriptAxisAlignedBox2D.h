#pragma once

#include "AxisAlignedBox2D.h"

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Spatial::AxisAlignedBox2D>
    {
        using Type = Spatial::AxisAlignedBox2D;
        using Management = ObjectManagement<Type>;

        static String Name() { return "AxisAlignedBox2D"; }
        static String ContainingNamespace() { return "Atmos::Spatial"; }
        static String Documentation() { return "2D axis aligned bounding box."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };
}