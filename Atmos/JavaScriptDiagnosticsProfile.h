#pragma once

#include "JavaScriptType.h"
#include "DiagnosticsStatistics.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Diagnostics::Statistics::Profile>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Diagnostics::Statistics::Profile& value);
        [[nodiscard]] static std::optional<Diagnostics::Statistics::Profile> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<double> time;
            Property<double> average;
            Property<double> highest;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}