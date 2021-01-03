#pragma once

#include "ActionDepressed.h"

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Input::ActionDepressed>
    {
        using Type = Input::ActionDepressed;
        using Management = ObjectManagement<Type>;

        static inline const String name = "ActionDepressed";
        static inline const String containingNamespace = "Atmos::Input";
        static inline const String documentation = "This is a signal.";
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };
}