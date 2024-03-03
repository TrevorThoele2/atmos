#pragma once

#include "JavaScriptType.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<bool>
    {
    public:
        using V8T = v8::Boolean;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const bool& value);
        [[nodiscard]] static std::optional<bool> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    };
}