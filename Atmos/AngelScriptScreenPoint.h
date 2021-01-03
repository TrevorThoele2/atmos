#pragma once

#include "ScreenPoint.h"

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Spatial::ScreenPoint>
    {
        using Type = Spatial::ScreenPoint;
        using Management = ObjectManagement<Type>;

        static inline const String name = "ScreenPoint";
        static inline const String containingNamespace = "Atmos::Spatial";
        static inline const String documentation = "Represents a point in screen coordinates.";
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };
}