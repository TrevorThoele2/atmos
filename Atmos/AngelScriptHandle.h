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
        static inline const String containingNamespace = "Arca";
        static inline const String documentation = "Represents a generic object handled by Arca. Can be a relic or a shard.";
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    public:
        static inline std::optional<int> typeID = {};
    };

    template<>
    struct Registration<Arca::SlimHandle> : Registration<Arca::Handle>
    {};
}