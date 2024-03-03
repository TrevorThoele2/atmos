#pragma once

#include "JavaScriptType.h"
#include "Variant.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Variant>
    {
    public:
        using V8T = v8::Value;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Variant& value);
        [[nodiscard]] static std::optional<Variant> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    };
}