#pragma once

#include "Scalers2D.h"

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Spatial::Scalers2D>
    {
        using Type = Spatial::Scalers2D;
        using Management = ObjectManagement<Type>;

        static inline const String name = "Scalers2D";
        static inline const String containingNamespace = "Atmos::Spatial";
        static inline const String documentation = "2D scalers.";
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };
}