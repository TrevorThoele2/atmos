#pragma once

#include "Point2D.h"

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Spatial::Point2D>
    {
        using Type = Spatial::Point2D;
        using Management = ObjectManagement<Type>;

        static inline const String name = "Point2D";
        static inline const String containingNamespace = "Atmos::Spatial";
        static inline const String documentation = "2D point.";
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };
}