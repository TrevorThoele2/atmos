#pragma once

#include "RenderCore.h"

#include "AngelScriptIndex.h"
#include "AngelScriptImageAsset.h"

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Render::RenderCore>
    {
        using Type = Arca::Index<Render::RenderCore>;
        using Management = ObjectManagement<Type>;

        static String Name() { return "RenderCore"; }
        static String ContainingNamespace() { return "Atmos::Render"; }
        static String Documentation() { return "This is a shard."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        [[nodiscard]] static Arca::Index<Asset::Material> Material(Type type);
        [[nodiscard]] static Render::Color Color(Type type);
    };

    template<>
    struct Registration<Arca::Index<Render::RenderCore>> : Registration<Render::RenderCore>
    {};
}