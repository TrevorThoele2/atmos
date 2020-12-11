#pragma once

#include "ImageMaterialAsset.h"
#include "LineMaterialAsset.h"
#include "RegionMaterialAsset.h"

#include "AngelScriptShaderAsset.h"

#include "AngelScriptIndex.h"
#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Asset::Material::Pass>
    {
        using Type = Asset::Material::Pass;
        using Management = ObjectManagement<Type>;

        static inline const String name = "MaterialPass";
        static inline const String containingNamespace = Namespaces::Atmos::Asset::name;
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine);
    };

    template<>
    struct Registration<Asset::ImageMaterial>
    {
        using Type = Arca::Index<Asset::ImageMaterial>;
        using Management = ObjectManagement<Type>;

        static inline const String name = "ImageMaterial";
        static inline const String containingNamespace = Namespaces::Atmos::Asset::name;
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine);
    private:
        [[nodiscard]] static String DoName(Type type);
        [[nodiscard]] static std::vector<Asset::Material::Pass> DoPasses(Type type);
    };

    template<>
    struct Registration<Asset::LineMaterial>
    {
        using Type = Arca::Index<Asset::LineMaterial>;
        using Management = ObjectManagement<Type>;

        static inline const String name = "LineMaterial";
        static inline const String containingNamespace = Namespaces::Atmos::Asset::name;
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine);
    private:
        [[nodiscard]] static String DoName(Type type);
        [[nodiscard]] static std::vector<Asset::Material::Pass> DoPasses(Type type);
    };

    template<>
    struct Registration<Asset::RegionMaterial>
    {
        using Type = Arca::Index<Asset::RegionMaterial>;
        using Management = ObjectManagement<Type>;

        static inline const String name = "RegionMaterial";
        static inline const String containingNamespace = Namespaces::Atmos::Asset::name;
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine);
    private:
        [[nodiscard]] static String DoName(Type type);
        [[nodiscard]] static std::vector<Asset::Material::Pass> DoPasses(Type type);
    };

    template<>
    struct Registration<Arca::Index<Asset::ImageMaterial>> : Registration<Asset::ImageMaterial>
    {};

    template<>
    struct Registration<Arca::Index<Asset::LineMaterial>> : Registration<Asset::LineMaterial>
    {};

    template<>
    struct Registration<Arca::Index<Asset::RegionMaterial>> : Registration<Asset::RegionMaterial>
    {};
}