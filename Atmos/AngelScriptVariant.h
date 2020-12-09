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

        static inline const String name = "Variant";
        static inline const String containingNamespace = Namespaces::Atmos::name;
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine);
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