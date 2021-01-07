#pragma once

#include "ActionPressed.h"

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Input::ActionPressed>
    {
        using Type = Input::ActionPressed;
        using Management = ObjectManagement<Type>;

        static String Name() { return "ActionPressed"; }
        static String ContainingNamespace() { return "Atmos::Input"; }
        static String Documentation() { return "This is a signal."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };
}