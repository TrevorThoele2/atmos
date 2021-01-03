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

        static inline const String name = "AxisAlignedBox2D";
        static inline const String containingNamespace = "Atmos::Spatial";
        static inline const String documentation = "2D axis aligned bounding box.";
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };
}