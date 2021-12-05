#pragma once

#include "JavaScriptType.h"
#include "LoggingSeverity.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Logging::Severity>
    {
    public:
        using V8T = v8::Number;

        [[nodiscard]] static v8::Local<v8::Value> Register(const RegistrationData& data);

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Logging::Severity& value);
        [[nodiscard]] static std::optional<Logging::Severity> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    };
}