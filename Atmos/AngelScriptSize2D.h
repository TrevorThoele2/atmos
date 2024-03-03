#pragma once

#include "Size2D.h"

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Spatial::Size2D>
    {
        using Type = Spatial::Size2D;
        using Management = ObjectManagement<Type>;

        static inline const String name = "Size2D";
        static inline const String containingNamespace = "Atmos::Spatial";
        static inline const String documentation = "2D size.";
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };
}