#pragma once

#include "JavaScriptType.h"
#include "String.h"

namespace Atmos::Scripting::JavaScript
{
    struct Symbol
    {
        std::optional<String> description;
    };

    template<>
    class Type<Symbol>
    {
    public:
        using V8T = v8::Symbol;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Symbol& value);
        [[nodiscard]] static std::optional<Symbol> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    };
}