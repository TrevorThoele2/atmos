#include "JavaScriptMoveBounds.h"

#include "JavaScriptNumeric.h"
#include "JavaScriptPoint3D.h"

#include "JavaScriptObject.h"
#include "JavaScriptUserData.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Spatial::MoveBounds>::ToV8(v8::Isolate& isolate, const Spatial::MoveBounds& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.id.SetValue(isolate, value.id);
        definition.to.SetValue(isolate, value.to);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Spatial::MoveBounds> Type<Spatial::MoveBounds>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Spatial::MoveBounds, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto id = definition.id.AtmosValue();
                const auto to = definition.to.AtmosValue();
                return id && to
                    ? Spatial::MoveBounds
                    {
                        *id, *to
                    }
                    : std::optional<Spatial::MoveBounds>{};
            });
    }

    Type<Spatial::MoveBounds>::Definition::Definition() :
        id("id"),
        to("to")
    {}

    AnyObjectDefinition Type<Spatial::MoveBounds>::Definition::ToAny()
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