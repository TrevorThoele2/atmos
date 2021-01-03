#pragma once

#include "Script.h"
#include "ModifyScriptData.h"

#include "AngelScriptScriptAsset.h"

#include "AngelScriptIndex.h"
#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

#include <Arca/Reliquary.h>

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Script>
    {
        using Type = Arca::Index<Script>;
        using Management = ObjectManagement<Type>;

        static inline const String name = "Script";
        static inline const String containingNamespace = "Atmos::Scripting";
        static inline const String documentation = "This is a relic.";
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        [[nodiscard]] static std::vector<Datum> Data(Type type);
    };

    template<>
    struct Registration<Arca::Index<Script>> : Registration<Script>
    {};

    template<>
    struct Registration<ModifyData>
    {
        using Type = ModifyData;
        using Management = ObjectManagement<Type>;

        static inline const String name = "ModifyData";
        static inline const String containingNamespace = "Atmos::Scripting";
        static inline const String documentation = "This is a command.";
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };
}