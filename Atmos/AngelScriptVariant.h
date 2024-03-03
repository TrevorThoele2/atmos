#pragma once

#include "Variant.h"

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

#include <Arca/Handle.h>

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Variant>
    {
        using Type = Variant;
        using Management = ObjectManagement<Type>;

        static String Name() { return "Variant"; }
        static String ContainingNamespace() { return "Atmos"; }
        static String Documentation()
        {
            return "Represents a general, serializable piece of data. Can only be one of its respective types at a time.";
        }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        [[nodiscard]] static bool AsBool(Type type);
        [[nodiscard]] static std::int64_t AsInt(Type type);
        [[nodiscard]] static double AsFloat(Type type);
        [[nodiscard]] static String AsString(Type type);
        [[nodiscard]] static bool IsBool(Type type);
        [[nodiscard]] static bool IsInt(Type type);
        [[nodiscard]] static bool IsFloat(Type type);
        [[nodiscard]] static bool IsString(Type type);
    };
}