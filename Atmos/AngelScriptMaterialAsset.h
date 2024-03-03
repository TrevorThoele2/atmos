#pragma once

#include "MaterialAsset.h"

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
    struct Registration<Asset::MaterialType>
    {
        using Type = Asset::MaterialType;

        static inline const String name = "MaterialType";
        static inline const String containingNamespace = Namespaces::Atmos::Asset::name;
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine);
    };

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
    struct Registration<Asset::Material>
    {
        using Type = Arca::Index<Asset::Material>;
        using Management = ObjectManagement<Type>;

        static inline const String name = "Material";
        static inline const String containingNamespace = Namespaces::Atmos::Asset::name;
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine);
    private:
        [[nodiscard]] static String DoName(Type type);
        [[nodiscard]] static Asset::MaterialType DoType(Type type);
        [[nodiscard]] static std::vector<Asset::Material::Pass> DoPasses(Type type);
    };

    template<>
    struct Registration<Arca::Index<Asset::Material>> : Registration<Asset::Material>
    {};
}