#pragma once

#include "ScriptVariant.h"

class asIScriptContext;

namespace Atmos::Script::Angel
{
    using ParameterIndex = unsigned int;

    void PushVariantToParameter(ParameterIndex index, Variant& variant, asIScriptContext* context);
}