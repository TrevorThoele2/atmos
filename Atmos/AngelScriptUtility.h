#pragma once

#include "ScriptVariant.h"

class asIScriptContext;

namespace Atmos::Script
{
    typedef unsigned int ParameterIndex;

    void PushVariantToAngelScriptParameter(ParameterIndex index, VariantT& variant, asIScriptContext* context);
}