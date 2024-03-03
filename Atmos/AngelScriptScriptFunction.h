#pragma once

#include "ScriptFunction.h"

class asIScriptFunction;

namespace Atmos::Scripting::Angel
{
    class Function final : public Scripting::Function
    {
    public:
        asIScriptFunction* function;
    };
}