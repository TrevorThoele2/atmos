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
    struct Registration<Asset::Material::Pass>
    {
        using Type = Asset::Material::Pass;
        using Management = ObjectManagement<Type>;

        static String Name() { return "MaterialPass"; }
        static String ContainingNamespace() { return "Atmos::Asset"; }
        static String Documentation() { return "Represents a pass for the material asset to perform."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };

    template<>
    struct Registration<Asset::Material>
    {
        using Type = Arca::Index<Asset::Material>;
        using Management = ObjectManagement<Type>;

        static String Name() { return "Material"; }
        static String ContainingNamespace() { return "Atmos::Asset"; }
        static String Documentation() { return "This is a relic."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        [[nodiscard]] static String DoName(Type type);
        [[nodiscard]] static std::vector<Asset::Material::Pass> DoPasses(Type type);
    };
    
    template<>
    struct Registration<Arca::Index<Asset::Material>> : Registration<Asset::Material>
    {};
}