#pragma once

#include "ScriptVariant.h"

class asIScriptContext;

namespace Atmos
{
    namespace Scripting
    {
        typedef unsigned int ParameterIndex;

        void PushVariantToAngelScriptParameter(ParameterIndex index, VariantT& variant, asIScriptContext* context);
    }
}