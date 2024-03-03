#pragma once

#include "DiagnosticsStatistics.h"

#include "AngelScriptIndex.h"
#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Diagnostics::Statistics>
    {
        using Type = Arca::Index<Diagnostics::Statistics>;
        using Management = ObjectManagement<Type>;

        static String Name() { return "Statistics"; }
        static String ContainingNamespace() { return "Atmos::Diagnostics"; }
        static String Documentation() { return "This is a relic."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        [[nodiscard]] static size_t RelicCount(Type type);
        [[nodiscard]] static size_t ShardCount(Type type);

        [[nodiscard]] static double RenderTime(Type type);
        [[nodiscard]] static double IdleTime(Type type);
    };

    template<>
    struct Registration<Arca::Index<Diagnostics::Statistics>> : Registration<Diagnostics::Statistics>
    {};
}