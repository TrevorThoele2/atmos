#include "JavaScriptDiagnosticsProfile.h"

#include "JavaScriptNumeric.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Diagnostics::Statistics::Profile>::ToV8(v8::Isolate& isolate, const Diagnostics::Statistics::Profile& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.time.SetValue(isolate, value.time);
        definition.average.SetValue(isolate, value.average);
        definition.highest.SetValue(isolate, value.highest);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Diagnostics::Statistics::Profile> Type<Diagnostics::Statistics::Profile>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Diagnostics::Statistics::Profile, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto time = definition.time.AtmosValue();
                const auto average = definition.average.AtmosValue();
                const auto highest = definition.highest.AtmosValue();
                return time && average && highest
                    ? Diagnostics::Statistics::Profile
                    {
                        *time, *average, *highest
                    }
                    : std::optional<Diagnostics::Statistics::Profile>{};
            });
    }

    Type<Diagnostics::Statistics::Profile>::Definition::Definition() :
        time("time"),
        average("average"),
        highest("highest")
    {}

    AnyObjectDefinition Type<Diagnostics::Statistics::Profile>::Definition::ToAny()
    {
        return
        {
            {
                { time },
                { average },
                { highest }
            }
        };
    }
}