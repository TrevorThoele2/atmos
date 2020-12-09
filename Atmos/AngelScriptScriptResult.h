#pragma once

#include "ScriptResult.h"

#include "AngelScriptIndex.h"
#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Result>
    {
        using Type = Result;
        using Management = ObjectManagement<Type>;

        static inline const String name = "Result";
        static inline const String containingNamespace = Namespaces::Atmos::Scripting::name;
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine);
    private:
        [[nodiscard]] static Variant AsVariant(Type type);
        [[nodiscard]] static bool IsVariant(Type type);
        [[nodiscard]] static bool IsFailure(Type type);
        [[nodiscard]] static bool IsQuit(Type type);
    };
}