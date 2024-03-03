#pragma once

#include <set>
#include <functional>
#include "String.h"

class asIScriptEngine;
class asIScriptGeneric;

namespace Arca
{
    class Reliquary;
}

namespace Atmos::Scripting::Angel
{
    class Manager;

    struct UserData
    {
        Arca::Reliquary* reliquary = nullptr;
        std::set<String> registeredTypes;

        using SignalToRegister = std::function<void(asIScriptEngine&, Arca::Reliquary&)>;
        std::vector<SignalToRegister> signalsToRegister;

        static UserData* RequiredFrom(asIScriptEngine& engine);
        static UserData* RequiredFrom(asIScriptGeneric& generic);
        static Arca::Reliquary* RequiredReliquaryFrom(asIScriptGeneric& generic);
        static std::vector<SignalToRegister>* RequiredSignalsToRegister(asIScriptEngine& engine);
    };
}