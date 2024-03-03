#pragma once

#include "TextEntered.h"

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Input::TextEntered>
    {
        using Type = Input::TextEntered;
        using Management = ObjectManagement<Type>;

        static String Name() { return "TextEntered"; }
        static String ContainingNamespace() { return "Atmos::Input"; }
        static String Documentation() { return "This is a signal."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };
}