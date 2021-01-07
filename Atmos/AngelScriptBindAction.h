#pragma once

#include "BindAction.h"

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Input::BindAction>
    {
        using Type = Input::BindAction;
        using Management = ObjectManagement<Type>;

        static String Name() { return "BindAction"; }
        static String ContainingNamespace() { return "Atmos::Input"; }
        static String Documentation() { return "This is a command."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        static void Constructor(asIScriptGeneric* generic);
    };
}