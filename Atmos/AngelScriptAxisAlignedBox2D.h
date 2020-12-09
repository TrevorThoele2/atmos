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
        static inline const String containingNamespace = Namespaces::Atmos::Spatial::name;
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine);
    };
}