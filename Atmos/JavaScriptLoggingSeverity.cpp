#include "JavaScriptLoggingSeverity.h"

#include "JavaScriptNumeric.h"
#include "JavaScriptEnum.h"

namespace Atmos::Scripting::JavaScript
{
    v8::Local<v8::Value> Type<Logging::Severity>::Register(const RegistrationData& data)
    {
        const auto isolate = data.isolate;

        return CreateEnum<Logging::Severity>(
            *isolate,
            {
                { "Verbose", Logging::Severity::Verbose },
                { "Information", Logging::Severity::Information },
                { "Warning", Logging::Severity::Warning },
                { "Error", Logging::Severity::Error }
            });
    }

    auto Type<Logging::Severity>::ToV8(v8::Isolate& isolate, const Logging::Severity& value) -> v8::Local<V8T>
    {
        return EnumType<Logging::Severity>::ToV8(isolate, value);
    }

    std::optional<Logging::Severity> Type<Logging::Severity>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return EnumType<Logging::Severity>::FromV8(isolate, value);
    }
}