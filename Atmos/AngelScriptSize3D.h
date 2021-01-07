#pragma once

#include "Size3D.h"

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Spatial::Size3D>
    {
        using Type = Spatial::Size3D;
        using Management = ObjectManagement<Type>;

        static String Name() { return "Size3D"; }
        static String ContainingNamespace() { return "Atmos::Spatial"; }
        static String Documentation() { return "3D size."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };
}