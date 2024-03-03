#pragma once

#include "AngelScriptNamespaces.h"
#include "AngelScriptObjectType.h"

class asIScriptEngine;

namespace Atmos::Scripting::Angel
{
    void RegisterAll(asIScriptEngine& engine);

    template<class T>
    struct Registration;
}