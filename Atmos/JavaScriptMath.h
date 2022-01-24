#pragma once

#include "JavaScriptType.h"
#include "Log.h"

namespace Atmos::Scripting::JavaScript
{
    struct MathNamespace
    {};

    template<>
    class Type<MathNamespace>
    {
    public:
        [[nodiscard]] static v8::Local<v8::Value> Register(const RegistrationData& data);
    };
}