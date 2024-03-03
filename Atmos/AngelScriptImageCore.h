#pragma once

#include "ImageCore.h"

#include "ChangeImageCore.h"

#include "AngelScriptIndex.h"
#include "AngelScriptImageAsset.h"
#include "AngelScriptMaterialAsset.h"

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    class ChangeImageAsset
    {
    public:
        Arca::RelicID id = 0;

        Arca::Index<Asset::Image> to;
    };

    class ChangeAssetIndex
    {
    public:
        Arca::RelicID id = 0;

        Render::ImageCore::Index to;
    };

    template<>
    struct Registration<Render::ImageCore>
    {
        using Type = Arca::Index<Render::ImageCore>;
        using Management = ObjectManagement<Type>;

        static String Name() { return "ImageCore"; }
        static String ContainingNamespace() { return "Atmos::Render"; }
        static String Documentation() { return "This is a shard."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        [[nodiscard]] static Arca::Index<Asset::Image> Asset(Type type);
        [[nodiscard]] static Render::ImageCore::Index AssetIndex(Type type);
        [[nodiscard]] static Arca::Index<Asset::ImageMaterial> Material(Type type);
        [[nodiscard]] static Render::Color Color(Type type);
    };

    template<>
    struct Registration<Arca::Index<Render::ImageCore>> : Registration<Render::ImageCore>
    {};

    template<>
    struct Registration<ChangeImageAsset>
    {
        using Type = ChangeImageAsset;

        static String Name() { return "ChangeImageAsset"; }
        static String ContainingNamespace() { return "Atmos::Render"; }
        static String Documentation() { return "This is a command."; }
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);

        static Render::ChangeImageCore ToArca(Type fromAngelScript);
    };

    template<>
    struct Registration<ChangeAssetIndex>
    {
        using Type = ChangeAssetIndex;

        static String Name() { return "ChangeAssetIndex"; }
        static String ContainingNamespace() { return "Atmos::Render"; }
        static String Documentation() { return "This is a command."; }
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);

        static Render::ChangeImageCore ToArca(Type fromAngelScript);
    };
}