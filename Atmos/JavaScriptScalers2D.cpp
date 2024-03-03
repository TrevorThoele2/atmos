#include "JavaScriptScalers2D.h"

#include "JavaScriptNumeric.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Spatial::Scalers2D>::ToV8(v8::Isolate& isolate, const Spatial::Scalers2D& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.x.SetValue(isolate, value.x);
        definition.y.SetValue(isolate, value.y);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Spatial::Scalers2D> Type<Spatial::Scalers2D>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Spatial::Scalers2D, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto x = definition.x.AtmosValue();
                const auto y = definition.y.AtmosValue();
                return x && y
                    ? Spatial::Scalers2D
	                {
	                    *x, *y
	                }
	                : std::optional<Spatial::Scalers2D>{};
            });
    }

    Type<Spatial::Scalers2D>::Definition::Definition() :
        x("x"),
        y("y")
    {}

    AnyObjectDefinition Type<Spatial::Scalers2D>::Definition::ToAny()
    {
        return
        {
            {
                { x },
                { y }
            }
        };
    }
}