#include "JavaScriptPoint2D.h"

#include "JavaScriptNumeric.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Spatial::Point2D>::ToV8(v8::Isolate& isolate, const Spatial::Point2D& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.x.SetValue(isolate, value.x);
        definition.y.SetValue(isolate, value.y);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Spatial::Point2D> Type<Spatial::Point2D>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Spatial::Point2D, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto x = definition.x.AtmosValue();
                const auto y = definition.y.AtmosValue();
                return x && y
                    ? Spatial::Point2D
	                {
	                    *x, *y
	                }
	                : std::optional<Spatial::Point2D>{};
            });
    }

    Type<Spatial::Point2D>::Definition::Definition() :
        x("x"),
        y("y")
    {}

    AnyObjectDefinition Type<Spatial::Point2D>::Definition::ToAny()
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