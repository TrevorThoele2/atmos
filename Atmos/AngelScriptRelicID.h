#pragma once

#include <Arca/RelicID.h>

#include "AngelScriptGlobalManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    class RelicID
    {};

    class NullRelicID
    {};

    template<>
    struct Registration<RelicID>
    {
        using Type = RelicID;
        using Management = GlobalManagement;

        static String Name() { return "RelicID"; }
        static String ContainingNamespace() { return "Arca"; }
        static String Documentation() { return "Every relic and shard has a RelicID that uniquely identifies it for that Reliquary."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };

    template<>
    struct Registration<NullRelicID>
    {
        using Type = NullRelicID;
        using Management = GlobalManagement;

        static String Name() { return "nullRelicID"; }
        static String ContainingNamespace() { return "Arca"; }
        static String Documentation() { return "The relic ID for a non-existent relic/shard."; }

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };
}