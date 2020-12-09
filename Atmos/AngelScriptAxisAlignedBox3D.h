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

        static inline const String name = "AxisAlignedBox3D";
        static inline const String containingNamespace = Namespaces::Atmos::Spatial::name;
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine);
    };
}