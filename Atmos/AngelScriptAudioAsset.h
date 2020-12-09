#pragma once

#include "AudioAsset.h"

#include "AngelScriptIndex.h"
#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Asset::Audio>
    {
        using Type = Arca::Index<Asset::Audio>;
        using Management = ObjectManagement<Type>;

        static inline const String name = "Audio";
        static inline const String containingNamespace = Namespaces::Atmos::Asset::name;
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine);
    private:
        [[nodiscard]] static String Name(Type type);
    };

    template<>
    struct Registration<Arca::Index<Asset::Audio>> : Registration<Asset::Audio>
    {};
}