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

    class ChangeColor
    {
    public:
        Arca::RelicID id = 0;

        Render::Color to;
    };

    class ChangeMaterialAsset
    {
    public:
        Arca::RelicID id = 0;

        Arca::Index<Asset::Material> to;
    };

    template<>
    struct Registration<Render::ImageCore>
    {
        using Type = Arca::Index<Render::ImageCore>;
        using Management = ObjectManagement<Type>;

        static inline const String name = "ImageCore";
        static inline const String containingNamespace = Namespaces::Atmos::Render::name;
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine);
    private:
        [[nodiscard]] static Arca::Index<Asset::Image> Asset(Type type);
        [[nodiscard]] static Render::ImageCore::Index AssetIndex(Type type);
        [[nodiscard]] static Arca::Index<Asset::Material> Material(Type type);
        [[nodiscard]] static Render::Color Color(Type type);
    };

    template<>
    struct Registration<Arca::Index<Render::ImageCore>> : Registration<Render::ImageCore>
    {};

    template<>
    struct Registration<ChangeImageAsset>
    {
        using Type = ChangeImageAsset;

        static inline const String name = "ChangeImageAsset";
        static inline const String containingNamespace = Namespaces::Atmos::Render::name;
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine);

        static Render::ChangeImageCore ToArca(Type fromAngelScript);
    };

    template<>
    struct Registration<ChangeAssetIndex>
    {
        using Type = ChangeAssetIndex;

        static inline const String name = "ChangeAssetIndex";
        static inline const String containingNamespace = Namespaces::Atmos::Render::name;
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine);

        static Render::ChangeImageCore ToArca(Type fromAngelScript);
    };

    template<>
    struct Registration<ChangeColor>
    {
        using Type = ChangeColor;

        static inline const String name = "ChangeColor";
        static inline const String containingNamespace = Namespaces::Atmos::Render::name;
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine);

        static Render::ChangeImageCore ToArca(Type fromAngelScript);
    };

    template<>
    struct Registration<ChangeMaterialAsset>
    {
        using Type = ChangeMaterialAsset;

        static inline const String name = "ChangeMaterialAsset";
        static inline const String containingNamespace = Namespaces::Atmos::Render::name;
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine);

        static Render::ChangeImageCore ToArca(Type fromAngelScript);
    };
}