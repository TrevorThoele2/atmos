#pragma once

#include "JavaScriptType.h"
#include "Log.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Logging::Log>
    {
    public:
        using V8T = v8::Object;
        
        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Logging::Log& value);
        [[nodiscard]] static std::optional<Logging::Log> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<String> message;
            Property<Logging::Severity> severity;
            Property<std::vector<NameValuePair>> details;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}