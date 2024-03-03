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
        static inline const String containingNamespace = "Atmos::Scripting";
        static inline const String documentation = "Represents the result of a finished script.";
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        [[nodiscard]] static Variant AsVariant(Type type);
        [[nodiscard]] static bool IsVariant(Type type);
        [[nodiscard]] static bool IsFailure(Type type);
        [[nodiscard]] static bool IsQuit(Type type);
    };
}