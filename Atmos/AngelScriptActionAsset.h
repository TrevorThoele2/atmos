#pragma once

#include "ActionAsset.h"

#include "AngelScriptIndex.h"
#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Asset::Action>
    {
        using Type = Arca::Index<Asset::Action>;
        using Management = ObjectManagement<Type>;

        static String Name() { return "Action"; }
        static String ContainingNamespace() { return "Atmos::Asset"; }
        static String Documentation() { return "This is a relic."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        [[nodiscard]] static String DoName(Type type);
        [[nodiscard]] static Input::Key DoBoundKey(Type type);
        [[nodiscard]] static std::vector<Input::Key> DoBoundModifiers(Type type);
    };

    template<>
    struct Registration<Arca::Index<Asset::Action>> : Registration<Asset::Action>
    {};
}