#pragma once

#include "Scalers3D.h"

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Spatial::Scalers3D>
    {
        using Type = Spatial::Scalers3D;
        using Management = ObjectManagement<Type>;

        static String Name() { return "Scalers3D"; }
        static String ContainingNamespace() { return "Atmos::Spatial"; }
        static String Documentation() { return "3D scalers."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };
}