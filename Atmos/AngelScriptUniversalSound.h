#pragma once

#include "UniversalSound.h"

#include "AngelScriptAudioAsset.h"

#include "AngelScriptIndex.h"
#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Audio::UniversalSound>
    {
        using Type = Arca::Index<Audio::UniversalSound>;
        using Management = ObjectManagement<Type>;

        static String Name() { return "UniversalSound"; }
        static String ContainingNamespace() { return "Atmos::Audio"; }
        static String Documentation() { return "This is a relic."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        [[nodiscard]] static Arca::Index<Asset::Audio> Asset(Type type);
        [[nodiscard]] static Audio::Volume Volume(Type type);
    };

    template<>
    struct Registration<Arca::Index<Audio::UniversalSound>> : Registration<Audio::UniversalSound>
    {};
}