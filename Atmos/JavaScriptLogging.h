#pragma once

#include "JavaScriptType.h"
#include "Log.h"

namespace Atmos::Scripting::JavaScript
{
    struct LoggingNamespace
    {};

    template<>
    class Type<LoggingNamespace>
    {
    public:
        [[nodiscard]] static v8::Local<v8::Value> Register(const RegistrationData& data);
        [[nodiscard]] static v8::Local<v8::Value> Traits(const RegistrationData& data);
    };
}