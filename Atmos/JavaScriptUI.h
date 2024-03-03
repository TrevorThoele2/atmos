#pragma once

#include "JavaScriptType.h"

namespace Atmos::Scripting::JavaScript
{
    struct UINamespace
    {};

    template<>
    class Type<UINamespace>
    {
    public:
        [[nodiscard]] static v8::Local<v8::Value> Traits(const RegistrationData& data);
    };
}