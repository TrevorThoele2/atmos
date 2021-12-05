#include "JavaScriptGridPoint.h"

#include "JavaScriptNumeric.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Spatial::Grid::Point>::ToV8(v8::Isolate& isolate, const Spatial::Grid::Point& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.x.SetValue(isolate, value.x);
        definition.y.SetValue(isolate, value.y);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Spatial::Grid::Point> Type<Spatial::Grid::Point>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Spatial::Grid::Point, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto x = definition.x.AtmosValue();
                const auto y = definition.y.AtmosValue();
                return x && y
                    ? Spatial::Grid::Point
                {
                    *x, *y
                }
                : std::optional<Spatial::Grid::Point>{};
            });
    }

    Type<Spatial::Grid::Point>::Definition::Definition() :
        x("x"),
        y("y")
    {}

    AnyObjectDefinition Type<Spatial::Grid::Point>::Definition::ToAny()
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