#pragma once

#include <Arca/Handle.h>

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Arca::Handle>
    {
        using Type = Arca::Handle;
        using Management = ObjectManagement<Type>;

        static inline const String name = "Handle";
        static inline const String containingNamespace = Namespaces::Arca::name;
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine);
    public:
        static inline std::optional<int> typeID = {};
    };

    template<>
    struct Registration<Arca::SlimHandle> : Registration<Arca::Handle>
    {};
}