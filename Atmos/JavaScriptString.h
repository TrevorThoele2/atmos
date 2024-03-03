#pragma once

#include "JavaScriptType.h"
#include "String.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<String>
    {
    public:
        using V8T = v8::String;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const String& value);
        [[nodiscard]] static std::optional<String> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    };
}