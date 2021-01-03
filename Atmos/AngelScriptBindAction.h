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

        static inline const String name = "BindAction";
        static inline const String containingNamespace = "Atmos::Input";
        static inline const String documentation = "This is a command.";
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        static void Constructor(asIScriptGeneric* generic);
    };
}