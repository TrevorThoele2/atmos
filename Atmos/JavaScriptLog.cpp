#include "JavaScriptLog.h"

#include "JavaScriptLoggingSeverity.h"
#include "JavaScriptString.h"
#include "JavaScriptNameValuePair.h"
#include "JavaScriptOptional.h"
#include "JavaScriptVector.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Logging::Log>::ToV8(v8::Isolate& isolate, const Logging::Log& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.message.SetValue(isolate, value.message);
        definition.severity.SetValue(isolate, value.severity);
        definition.details.SetValue(isolate, value.details);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Logging::Log> Type<Logging::Log>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Logging::Log, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto message = definition.message.AtmosValue();
                const auto severity = definition.severity.AtmosValue();
                const auto details = definition.details.AtmosValue();
                return message
                    ? Logging::Log
	                {
	                    *message,
	                    severity ? *severity : Logging::Severity::Information,
	                    details ? *details : std::vector<NameValuePair>{}
	                }
	                : std::optional<Logging::Log>{};
            });
    }
    
    Type<Logging::Log>::Definition::Definition() :
		message("message"),
		severity("severity"),
		details("details")
    {}

	AnyObjectDefinition Type<Logging::Log>::Definition::ToAny()
    {
        return
        {
            {
	            { message },
                { severity },
                { details }
            }
        };
    }
}