#pragma once

#include "Script.h"

#include "AngelScriptScriptAsset.h"

#include "AngelScriptIndex.h"
#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

#include <Arca/Reliquary.h>

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Script>
    {
        using Type = Arca::Index<Script>;
        using Management = ObjectManagement<Type>;

        static String Name() { return "Script"; }
        static String ContainingNamespace() { return "Atmos::Scripting"; }
        static String Documentation() { return "This is a relic."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };

    template<>
    struct Registration<Arca::Index<Script>> : Registration<Script>
    {};
}