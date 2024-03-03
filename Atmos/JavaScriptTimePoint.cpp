#include "JavaScriptTimePoint.h"

#include "JavaScriptNumeric.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Time::Point<>>::ToV8(v8::Isolate& isolate, const Time::Point<>& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.nanoseconds.SetValue(isolate, value.time_since_epoch().count());
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Time::Point<>> Type<Time::Point<>>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Time::Point<>, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto nanoseconds = definition.nanoseconds.AtmosValue();
                return nanoseconds
                    ? Time::Point<>
                    {
                        Time::Nanoseconds{ *nanoseconds }
                    }
                    : std::optional<Time::Point<>>{};
            });
    }

    Type<Time::Point<>>::Definition::Definition() :
        nanoseconds("nanoseconds")
    {}

    AnyObjectDefinition Type<Time::Point<>>::Definition::ToAny()
    {
        return
        {
            {
                { nanoseconds }
            }
        };
    }
}