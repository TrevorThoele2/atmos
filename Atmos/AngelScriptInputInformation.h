#pragma once

#include "InputInformation.h"

#include "AngelScriptIndex.h"
#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"
#include "Point2D.h"

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Input::Information>
    {
        using Type = Arca::Index<Input::Information>;
        using Management = ObjectManagement<Type>;

        static String Name() { return "Information"; }
        static String ContainingNamespace() { return "Atmos::Input"; }
        static String Documentation() { return "This is a relic."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        [[nodiscard]] static Spatial::Point2D PreviousMousePosition(Type type);
        [[nodiscard]] static Spatial::Point2D CurrentMousePosition(Type type);
    };

    template<>
    struct Registration<Arca::Index<Input::Information>> : Registration<Input::Information>
    {};
}