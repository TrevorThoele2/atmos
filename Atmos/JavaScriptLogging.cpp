#include "JavaScriptLogging.h"

#include "JavaScriptLog.h"
#include "JavaScriptLoggingSeverity.h"
#include "JavaScriptTraits.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    v8::Local<v8::Value> Type<LoggingNamespace>::Register(const RegistrationData& data)
    {
        const auto isolate = data.isolate;

        return CreateObject(
            *isolate,
            {
                { "Severity", Type<Logging::Severity>::Register(data) }
            });
    }

	v8::Local<v8::Value> Type<LoggingNamespace>::Traits(const RegistrationData& data)
    {
        const auto isolate = data.isolate;

        return CreateObject(
            *isolate,
            {
                { "Log", ToV8(*isolate, ArcaTraits::From<Logging::Log>()) }
            });
    }
}