#pragma once

#include "PositionedSound.h"

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
    struct Registration<Audio::PositionedSound>
    {
        using Type = Arca::Index<Audio::PositionedSound>;
        using Management = ObjectManagement<Type>;

        static String Name() { return "PositionedSound"; }
        static String ContainingNamespace() { return "Atmos::Audio"; }
        static String Documentation() { return "This is a relic."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        [[nodiscard]] static Arca::Index<Asset::Audio> Asset(Type type);
        [[nodiscard]] static Audio::Volume Volume(Type type);
    };

    template<>
    struct Registration<Arca::Index<Audio::PositionedSound>> : Registration<Audio::PositionedSound>
    {};
}