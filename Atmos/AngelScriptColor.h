#pragma once

#include "Color.h"
#include "ChangeColor.h"

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

        static String Name() { return "Color"; }
        static String ContainingNamespace() { return "Atmos::Render"; }
        static String Documentation() { return "Represents a color."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };

    template<>
    struct Registration<Render::ChangeColor>
    {
        using Type = Render::ChangeColor;

        static String Name() { return "ChangeColor"; }
        static String ContainingNamespace() { return "Atmos::Render"; }
        static String Documentation() { return "This is a command."; }
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };
}