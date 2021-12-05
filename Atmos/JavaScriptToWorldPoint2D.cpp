#include "JavaScriptToWorldPoint2D.h"

#include "JavaScriptPoint2D.h"
#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Spatial::ToWorldPoint2D>::ToV8(v8::Isolate& isolate, const Spatial::ToWorldPoint2D& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.from.SetValue(isolate, value.from);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Spatial::ToWorldPoint2D> Type<Spatial::ToWorldPoint2D>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Spatial::ToWorldPoint2D, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto from = definition.from.AtmosValue();
                return from
                    ? Spatial::ToWorldPoint2D
                    {
                        *from
                    }
                    : std::optional<Spatial::ToWorldPoint2D>{};
            });
    }

    Type<Spatial::ToWorldPoint2D>::Definition::Definition() :
        from("from")
    {}

    AnyObjectDefinition Type<Spatial::ToWorldPoint2D>::Definition::ToAny()
    {
        return
        {
            {
                { from }
            }
        };
    }
}