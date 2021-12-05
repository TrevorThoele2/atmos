#pragma once

#include "JavaScriptType.h"
#include "Variant.h"

namespace Atmos::Scripting::JavaScript
{
    struct Result
    {
        bool done;
        std::optional<Variant> value;
    };

    template<>
    class Type<Result>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Result& value);
        [[nodiscard]] static std::optional<Result> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    };
}