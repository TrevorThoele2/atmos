#pragma once

#include <set>
#include <map>
#include <memory>
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

    struct PerScriptUserData
    {
        
    };

    struct UserData
    {
        Arca::Reliquary* reliquary = nullptr;
        Manager* manager = nullptr;
        std::set<String> registeredTypes;

        using PerScriptUserDataPtr = std::unique_ptr<PerScriptUserData>;
        std::map<size_t, PerScriptUserDataPtr> perScripts;

        static UserData* RequiredFrom(asIScriptEngine& engine);
        static UserData* RequiredFrom(asIScriptGeneric& generic);
        static Arca::Reliquary* RequiredReliquaryFrom(asIScriptGeneric& generic);
    };
}