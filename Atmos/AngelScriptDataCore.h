#pragma once

#include "DataCore.h"

#include "AngelScriptIndex.h"

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<DataCore>
    {
        using Type = Arca::Index<DataCore>;
        using Management = ObjectManagement<Type>;

        static String Name() { return "DataCore"; }
        static String ContainingNamespace() { return "Atmos"; }
        static String Documentation() { return "This is a shard."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        [[nodiscard]] static std::vector<Property> Properties(Type type);
    };

    template<>
    struct Registration<Arca::Index<DataCore>> : Registration<DataCore>
    {};
}