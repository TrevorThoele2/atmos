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

        static inline const String name = "Stopwatch";
        static inline const String containingNamespace = Namespaces::Atmos::Time::name;
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine);
    private:
        [[nodiscard]] static Time::Stopwatch CreateFrameStopwatch(Arca::Reliquary* reliquary);
    };
}