#pragma once

#include <vector>

class asIScriptEngine;
class asIScriptGeneric;
class asIScriptFunction;

namespace Arca
{
    class Reliquary;
}

namespace Atmos::Scripting::Angel
{
    class Manager;

    struct ArcaUserData
    {
        std::vector<asIScriptFunction*> functions;

        static ArcaUserData* RequiredFrom(asIScriptEngine& engine);
        static ArcaUserData* RequiredFrom(asIScriptGeneric& generic);
        static std::vector<asIScriptFunction*>* RequiredFunctionsFrom(asIScriptGeneric& generic);
    };
}