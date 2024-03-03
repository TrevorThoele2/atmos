#include "JavaScriptRotateBounds.h"

#include "JavaScriptNumeric.h"

#include "JavaScriptObject.h"
#include "JavaScriptUserData.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Spatial::RotateBounds>::ToV8(v8::Isolate& isolate, const Spatial::RotateBounds& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.id.SetValue(isolate, value.id);
        definition.to.SetValue(isolate, value.to);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Spatial::RotateBounds> Type<Spatial::RotateBounds>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Spatial::RotateBounds, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto id = definition.id.AtmosValue();
                const auto to = definition.to.AtmosValue();
                return id && to
                    ? Spatial::RotateBounds
                {
                    *id, *to
                }
                : std::optional<Spatial::RotateBounds>{};
            });
    }

    Type<Spatial::RotateBounds>::Definition::Definition() :
        id("id"),
        to("to")
    {}

    AnyObjectDefinition Type<Spatial::RotateBounds>::Definition::ToAny()
    {
        return
        {
            {
                { id },
                { to }
            }
        };
    }
}