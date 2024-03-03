#include "JavaScriptGridSize.h"

#include "JavaScriptNumeric.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Spatial::Grid::Size>::ToV8(v8::Isolate& isolate, const Spatial::Grid::Size& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.width.SetValue(isolate, value.width);
        definition.height.SetValue(isolate, value.height);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Spatial::Grid::Size> Type<Spatial::Grid::Size>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Spatial::Grid::Size, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto width = definition.width.AtmosValue();
                const auto height = definition.height.AtmosValue();
                return width && height
                    ? Spatial::Grid::Size
                {
                    *width, *height
                }
                : std::optional<Spatial::Grid::Size>{};
            });
    }

    Type<Spatial::Grid::Size>::Definition::Definition() :
        width("width"),
        height("height")
    {}

    AnyObjectDefinition Type<Spatial::Grid::Size>::Definition::ToAny()
    {
        return
        {
            {
                { width },
                { height }
            }
        };
    }
}