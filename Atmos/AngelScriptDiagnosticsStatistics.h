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
    struct Registration<Diagnostics::Statistics::Profile>
    {
        using Type = Diagnostics::Statistics::Profile;
        using Management = ObjectManagement<Type>;

        static String Name() { return "Profile"; }
        static String ContainingNamespace() { return "Atmos::Diagnostics::Statistics"; }
        static String Documentation() { return "Profile for a specific time block."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        [[nodiscard]] static double Time(Type type);
        [[nodiscard]] static double Average(Type type);
        [[nodiscard]] static double Highest(Type type);
    };

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
        [[nodiscard]] static Arca::RelicID NextRelicID(Type type);

        [[nodiscard]] static Diagnostics::Statistics::Profile Script(Type type);
        [[nodiscard]] static Diagnostics::Statistics::Profile Render(Type type);
        [[nodiscard]] static Diagnostics::Statistics::Profile Frame(Type type);
        [[nodiscard]] static Diagnostics::Statistics::Profile Idle(Type type);
        [[nodiscard]] static Diagnostics::Statistics::Profile Misc(Type type);

        [[nodiscard]] static int FramesPerSecond(Type type);
    };

    template<>
    struct Registration<Arca::Index<Diagnostics::Statistics>> : Registration<Diagnostics::Statistics>
    {};
}