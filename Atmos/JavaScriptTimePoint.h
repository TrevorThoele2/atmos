#pragma once

#include "JavaScriptType.h"
#include "TimePoint.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Time::Point<>>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Time::Point<>& value);
        [[nodiscard]] static std::optional<Time::Point<>> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<long long> nanoseconds;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}