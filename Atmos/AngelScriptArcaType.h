#pragma once

#include <Arca/Type.h>

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Arca::Type>
    {
        using Type = Arca::Type;
        using Management = ObjectManagement<Type>;

        static inline const String name = "Type";
        static inline const String containingNamespace = Namespaces::Arca::name;
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine);
    private:
        [[nodiscard]] static String Name(Type type);
    };
}