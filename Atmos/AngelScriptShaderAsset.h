#pragma once

#include "ShaderAsset.h"

#include "AngelScriptIndex.h"
#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Asset::Shader>
    {
        using Type = Arca::Index<Asset::Shader>;
        using Management = ObjectManagement<Type>;

        static inline const String name = "Shader";
        static inline const String containingNamespace = "Atmos::Asset";
        static inline const String documentation = "This is a relic.";
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        [[nodiscard]] static String Name(Type type);
    };

    template<>
    struct Registration<Arca::Index<Asset::Shader>> : Registration<Asset::Shader>
    {};
}