#pragma once

#include "FontAsset.h"

#include "AngelScriptIndex.h"
#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Asset::Font>
    {
        using Type = Arca::Index<Asset::Font>;
        using Management = ObjectManagement<Type>;

        static String Name() { return "Font"; }
        static String ContainingNamespace() { return "Atmos::Asset"; }
        static String Documentation() { return "This is a relic."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        [[nodiscard]] static Spatial::Size2D Size(Type type, String string, bool bold, bool italics);
    };

    template<>
    struct Registration<Arca::Index<Asset::Font>> : Registration<Asset::Font>
    {};
}