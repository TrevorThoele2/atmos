#pragma once

#include "JavaScriptType.h"

namespace Atmos::Scripting::JavaScript
{
    struct InputNamespace
    {};

    template<>
    class Type<InputNamespace>
    {
    public:
        [[nodiscard]] static v8::Local<v8::Value> Traits(const RegistrationData& data);
    };
}