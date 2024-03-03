#pragma once

#include "Stopwatch.h"

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Time::Stopwatch>
    {
        using Type = Time::Stopwatch;
        using Management = ObjectManagement<Type>;

        static String Name() { return "Stopwatch"; }
        static String ContainingNamespace() { return "Atmos::Time"; }
        static String Documentation() { return "This is a relic."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        [[nodiscard]] static Time::Stopwatch CreateFrameStopwatch(Arca::Reliquary* reliquary);
    };
}