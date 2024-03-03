#pragma once

#include "GridPoint.h"

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Spatial::Grid::Point>
    {
        using Type = Spatial::Grid::Point;
        using Management = ObjectManagement<Type>;

        static inline const String name = "Point";
        static inline const String containingNamespace = Namespaces::Atmos::Spatial::Grid::name;
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine);
    };
}