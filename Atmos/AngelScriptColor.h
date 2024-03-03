#pragma once

#include "Color.h"

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Render::Color>
    {
        using Type = Render::Color;
        using Management = ObjectManagement<Type>;

        static inline const String name = "Color";
        static inline const String containingNamespace = Namespaces::Atmos::Render::name;
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine);
    };
}