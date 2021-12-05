#include "JavaScriptScaleBounds.h"

#include "JavaScriptNumeric.h"
#include "JavaScriptScalers2D.h"

#include "JavaScriptObject.h"
#include "JavaScriptUserData.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Spatial::ScaleBounds>::ToV8(v8::Isolate& isolate, const Spatial::ScaleBounds& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.id.SetValue(isolate, value.id);
        definition.to.SetValue(isolate, value.to);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Spatial::ScaleBounds> Type<Spatial::ScaleBounds>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Spatial::ScaleBounds, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto id = definition.id.AtmosValue();
                const auto to = definition.to.AtmosValue();
                return id && to
                    ? Spatial::ScaleBounds
                    {
                        *id, *to
                    }
                    : std::optional<Spatial::ScaleBounds>{};
            });
    }

    Type<Spatial::ScaleBounds>::Definition::Definition() :
        id("id"),
        to("to")
    {}

    AnyObjectDefinition Type<Spatial::ScaleBounds>::Definition::ToAny()
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