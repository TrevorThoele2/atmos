#pragma once

#include "JavaScriptType.h"

namespace Atmos::Scripting::JavaScript
{
    struct ScriptingNamespace
    {};

    template<>
    class Type<ScriptingNamespace>
    {
    public:
        [[nodiscard]] static v8::Local<v8::Value> Traits(const RegistrationData& data);
    };
}