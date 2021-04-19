#pragma once

#include "AudioAsset.h"

#include "AngelScriptIndex.h"
#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Asset::Audio>
    {
        using Type = Arca::Index<Asset::Audio>;
        using Management = ObjectManagement<Type>;

        static String Name() { return "Audio"; }
        static String ContainingNamespace() { return "Atmos::Asset"; }
        static String Documentation() { return "This is a relic."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        [[nodiscard]] static String DoName(Type type);
    };

    template<>
    struct Registration<Arca::Index<Asset::Audio>> : Registration<Asset::Audio>
    {};
}