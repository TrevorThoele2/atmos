#pragma once

#include "Angle2D.h"

#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;

namespace Atmos::Scripting::Angel
{
    class Angle2D
    {};

    template<>
    struct Registration<Angle2D>
    {
        static inline const String name = "Angle2D";
        static inline const String containingNamespace = Namespaces::Atmos::Spatial::name;
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine);
    };
}